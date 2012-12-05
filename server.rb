require 'rubygems'
require 'sinatra'
require 'sinatra/cross_origin'
require 'json' 
require 'serialport'
require 'thread'
require 'time'

state = 'pause'

vid_length = 0
arduino_buffer = {}
send_to_client = ''
client_buffer = {}

def getLast(hash)
	if hash.length <1 
		return 0
	end
	return hash.sort_by{|time, value| time}[-1][1] # sort ascending time
end

	

def sendSync(sp, client_buffer, arduino_buffer)
	client = getLast(client_buffer)
	server = getLast(arduino_buffer)
	
	client = client.to_f
	server = server.to_f
	if server == 0.0 # save from divide by zero
		server = 1
	end

        sp.write("null -")

end


def getMessage(sp)
 	a = ''	
	b = '1'
	while (b != '-') # keep reading till end char '-'
		b = sp.getc
		if (b!=nil)
			a += b
		end
	end
	return a       
end
Thread.new do
	sp = SerialPort.new "/dev/ttyACM0", 9600
        
        while (vid_length == 0) # spin till video length is set by client
        end
        
        puts "length is now " + vid_length.to_s
        str =  'length ' + vid_length.to_s + ' -'
        sp.write(str)

       	while 1
                a = getMessage(sp)

                if a.include?('pause')
                state = 'pause'
			pos = a[/\d+(?:\.\d+)?/]
                        pos = pos.to_f/3328.0 
                        #puts "a: " + a.to_s
                        puts "pos is : " + pos.to_s
                        pos = pos.to_f
			if pos != nil
				arduino_buffer[Time.now.iso8601] = pos
			end
		end
                
		if a.include?('play')
			state = 'sync'
			pos = a[/\d+(?:\.\d+)?/]
                        puts "a: " + a.to_s
                        puts "pos is : " + pos.to_s
                        pos = pos.to_f/3328.0
			if pos != nil
				arduino_buffer[Time.now.iso8601] = pos
			end
		end
                
                if a.include?('motion')
                        state = 'motion'
	        	pos = a[/\d+(?:\.\d+)?/]
                        puts "a: " + a.to_s
                        puts "pos is : " + pos.to_s
                        pos = pos.to_f/3328.0
	        end

		sendSync(sp, client_buffer, arduino_buffer)
	end # end while loop
end

Tilt.register Tilt::ERBTemplate, 'html.erb'
def herb(template, options={}, locals={})
  render "html.erb", template, options, locals
end

configure do
  enable :cross_origin
end

options '/*' do 
	response["Access-Control-Allow-Headers"] = "origin, x-requested-with, content-type"
end


get '/' do 
	herb :index
end

get '/state' do
	content_type :json
	{state: state, pos:getLast(arduino_buffer) }.to_json
end

post '/length' do
        vid_length = params[:len] 
end

post '/position' do
	 
	cli_pos = params[:pos]  #pos is number 0 - 1
        cli_pos = cli_pos.to_f
	client_buffer[Time.now.iso8601] = cli_pos
end

