require 'rubygems'
require 'sinatra'
require 'json' 
require 'serialport'
require 'thread'
require 'time'

state = 'sync'

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
	
	client = client.to_i
	server = server.to_i
	if server == 0
		server = 1
	end

	if ((server-client).abs/server * 100) < 2
		sp.write('sync ' + getLast(client_buffer).to_s() + ' -')
	else
		sp.write("null -")
	end
end

Thread.new do 
	
	sp = SerialPort.new "/dev/ttyACM1", 9600

	while 1

		a = ''	
		b = '1'
		while (b != '-') # keep reading till end char '-'
			b = sp.getc
			if (b!=nil)
				a += b
			end
		end
		# 'a' is the string read from arduino

		if a.include?('pause')
			state = 'pause'
			pos = a[/\d+(?:\.\d+)?/]
			if pos != nil
				arduino_buffer[Time.now.iso8601] = pos
			end
		end

		if a.include?('play')
			state = 'sync'
			pos = a[/\d+(?:\.\d+)?/]
			if pos != nil
				arduino_buffer[Time.now.iso8601] = pos
			end
		end
	
		sendSync(sp, client_buffer, arduino_buffer)

	end # end while loop
end

Tilt.register Tilt::ERBTemplate, 'html.erb'
def herb(template, options={}, locals={})
  render "html.erb", template, options, locals
end

get '/' do 
	herb :index
end

get '/state' do
	content_type :json
	{state: state, pos:getLast(arduino_buffer) }.to_json
end

post '/sync' do 
	pos = params[:pos]  #pos is number 0 - 1
	client_buffer[Time.now.iso8601] = pos
end
