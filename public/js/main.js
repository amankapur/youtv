var pos, lastpos = 0; //These need to be in an object with structure
var serverLoop;
var playerObj = null;

$(document).on('ready', function(){
    var dev_key =  "AI39si6p8JyCYDoSBE6Fcv16d7Xykw_trX4LVPHooYk9Y5uaY3VlveaH3XYMJO-El2gcQ1J8woIsa1-lGzyBMtmD6uCmu1FJ_w"
    $('#search').submit(function() {
        $("#yt_vids").html('');
        // get all the inputs into an array.
        var inputs = $('#search :input');
        var query = inputs.serializeArray()[0].value;
        var url = "https://gdata.youtube.com/feeds/api/videos?q=" + encodeURI(query) + "&key=" + dev_key;
        $.get(url, function(data){
            var entries = $(data).find('entry');
            console.log(data);
            window.d = data;
            for (var vid =0; vid < 10; vid++){
                var entry = $(entries[vid]);
                var thumbnail = entry.find("thumbnail")[1];
                var thumbnail_url = thumbnail.getAttribute('url');
                var temp = entry.find("content")[1];
                var vid_url = temp.getAttribute('url');
                var vid_id = youtube_id_from_url(vid_url);
                var title = entry.find('title').text();
                var html = '<div class="row"><a href="#"><img src = ' + thumbnail_url + ' onClick="loadPlayer(\'' + vid_id + '\')"></a> <h2>'+ title + ' </h2> </div>';
                $("#yt_vids").append(html);
            }
        });
    return false;
    });
});

window.onkeydown = fullScreenPlayer;
//window.addEventListener('onkeydown', function (keyEvent) {
function fullScreenPlayer(keyEvent) {
    console.log('a');
    var keyCode = ('which' in event) ? event.which : event.keyCode;
    console.log(keyCode);
    if (keyCode == 70 && playerObj) {
        //playerObj.requestFullScreen();
        fullScreenApi.requestFullScreen(playerObj);
    }
}

function GetChar (event){
            var keyCode = ('which' in event) ? event.which : event.keyCode;
            alert ("The Unicode key code is: " + keyCode);
        }

function loadPlayer(vid_id) {
    // Lets Flash from another domain call JavaScript
    var params = { allowScriptAccess: "always" };
    // The element id of the Flash embed
    var atts = { id: "ytPlayer" };
    swfobject.embedSWF("http://www.youtube.com/v/" + vid_id + "?version=3&enablejsapi=1&playerapiid=player1", 
                       "videoDiv", "480", "295", "9", null, null, params, atts);
    playerObj = $("object#ytPlayer");

    postLength();

    window.clearInterval(serverLoop); //This really needs to be placed where the video is closed, not here.
    serverLoop = window.setInterval(function(){
        getState();
        postSync();
        //postLength();
    }, 100);
    $("videoDiv")
}

function getState() {
    $.ajax({
        type: "GET",
        url: "/state",
        dataType: "json",
        success: function(data){
            //console.log(data);
            lastpos = pos;
            pos = data.pos;
            if (Math.abs(pos-lastpos) > 5) {
                //The pos has jumped more than expected, tell video to seek 
            }
        }
    });
}

function postSync() {
    $.ajax({
        type: "POST",
        url: "/sync",
        dataType: "json",
        data: {"pos" : 0}
    });
}

//Post video length in seconds
function postLength() {
    $.ajax({
        type: "POST",
        url: "/length",
        dataType: "json",
        data: {"len":100}
    });
}

//NOT http://stackoverflow.com/questions/6556559/youtube-api-extract-video-id
//http://stackoverflow.com/questions/3452546/javascript-regex-how-to-get-youtube-video-id-from-url
function youtube_id_from_url(url) {
    var video_id = url.split('v/')[1];
    var ampersandPosition = video_id.indexOf('?');
    if(ampersandPosition != -1) {
        video_id = video_id.substring(0, ampersandPosition);
    }
    return video_id;
}
