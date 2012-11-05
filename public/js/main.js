$(document).on('ready', function(){
	var dev_key =  "AI39si6p8JyCYDoSBE6Fcv16d7Xykw_trX4LVPHooYk9Y5uaY3VlveaH3XYMJO-El2gcQ1J8woIsa1-lGzyBMtmD6uCmu1FJ_w"
	$('#search').submit(function() {
		$("#yt_vids").html('');
    // get all the inputs into an array.
    var inputs = $('#search :input');

  	var query = inputs.serializeArray()[0].value;
  	
  	
  	var url = "https://gdata.youtube.com/feeds/api/videos?q=" + query + "&key=" + dev_key;
		console.log(url);
		$.get(url, function(data){

			var entries = $(data).find('entry');
			console.log(entries);
  		for (var vid =0; vid < 3; vid++){
  			var entry = $(entries[vid]);
  			console.log(entry);
//  			console.log(entry.find("content"));
  			var temp = entry.find("content")[1];
  			console.log(temp);
  			var vid_url = temp.getAttribute('url');
  			console.log(url);
  			var embed_url = "<object width='560' height='315'><param name='movie' value='" + vid_url + ";hl=en_US'></param><param name='allowFullScreen' value='true'></param><param name='allowscriptaccess' value='always'></param><embed src='"+ vid_url + "http://www.youtube.com/v/-kys635B4H8?version=3&amp;hl=en_US' type='application/x-shockwave-flash' width='560' height='315' allowscriptaccess='always' allowfullscreen='true'></embed></object>"
  			
  			var html = '<div class="row"> ' +embed_url +'</div>';
  			$("#yt_vids").append(html);
  			
  		}
  			
		});

  	
		return false;
		});
});
