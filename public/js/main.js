var pos, lastpos = 0; //These need to be in an object with structure

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
                var title = entry.find('title').text();
                var html = '<div class="row"> <a href=' + vid_url + '><img src = ' + thumbnail_url + '></a> <h2>'+ title+' </h2> </div>';
                $("#yt_vids").append(html);
            }
        });
    return false;
    });
    window.setInterval(function(){
        getState();
    }, 100);

function getState() {
    $.ajax({
        type: "GET",
        url: "/state",
        dataType: "json",
        crossDomain: true,
        success: function(data){
            console.log("Did that just work?");
        }
    });
   }
    /*$.ajax({
        type: 
    })*/

    /*$.getJSON('localhost:4567/state', function(data){
        alert('got it');
        lastpos = pos;
        pos = data.pos;

        if (Math.abs(pos-lastpos) > 5) {
          //The pos has jumped more than expected, tell video to seek 
        }
    });*/


function postSync() {
  $.post(host+'/sync')
}

//Post video length in seconds
function postLength() {
  $.post(host+'/length')
}

});
