//var pos, lastpos = 0; //These need to be in an object with structure
//var state, laststate = null;
var serverLoop;
var playerObj = null;
var length, position = 0.0;

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

function loadPlayer(vid_id) {
    // Lets Flash from another domain call JavaScript
    var params = { allowScriptAccess: "always" };
    // The element id of the Flash embed
    var atts = { id: "ytPlayer" };
    swfobject.embedSWF("http://www.youtube.com/v/" + vid_id + "?version=3&enablejsapi=1&playerapiid=ytPlayer", 
                       "videoDiv", $(window).width().toString(), $(window).height().toString(), "9", null, null, params, atts);
    playerObj = $("object#ytPlayer");
    toggleFullScreen();
}

function onYouTubePlayerReady(playerid) {
    postLength();
    serverLoop = window.setInterval(function(){
        getState();
        //postSync();
        //postLength();
    }, 100);
}

function closePlayer() {
    window.clearInterval(serverLoop);
    $("#ytPlayer").replaceWith("<div id=\"videoDiv\"></div>")
}
    
function toggleFullScreen() {
    var videoElement = document.getElementById("ytPlayer");
    if (!document.mozFullScreen && !document.webkitFullScreen) {
        if (videoElement.mozRequestFullScreen) {
            videoElement.mozRequestFullScreen();
        } else {
            videoElement.webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT);
        }
    } else {
        if (document.mozCancelFullScreen) {
            document.mozCancelFullScreen();
        } else {
            document.webkitCancelFullScreen();
        }
    }
}

function getState() {
    $.ajax({
        type: "GET",
        url: "/state",
        dataType: "json",
        success: function(data){
            console.log(data.pos+','+data.state);
            //lastpos = pos;
            //pos = data.pos;
            //if (Math.abs(pos-lastpos) > 5) {
                //The pos has jumped more than expected, tell video to seek 
            //}
            vidState = ytPlayer.getPlayerState();
            if (data.state == "play" && vidState != 1 && vidState != 0) {
                //The video should be playing
                if (state == "play") ytPlayer.playVideo();
            }
            else if (data.state == "pause" && vidState != 2) {
                ytPlayer.pauseVideo();
            }
        }
    });
}

function postSync() {
    if (length) {
        position = ytPlayer.getCurrentTime()/length;
    }
    else position = ytPlayer.getCurrentTime()/ytPlayer.getDuration();
    $.ajax({
        type: "POST",
        url: "/sync",
        dataType: "json",
        data: {"pos": "0.0"}
    });
}

//Post video length in seconds
function postLength() {
    length = ytPlayer.getDuration();
    $.ajax({
        type: "POST",
        url: "/length",
        dataType: "json",
        data: {"len":length}
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
