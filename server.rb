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

thread = Thread.new(){
	sp = SerialPort.new "/dev/ttyACM0", 9600
	sp.write('hi')
	puts 'hi'

	while(1)
		a = sp.read()
		if a.include?('pause')
			state = 'pause'
			pos = a.split('|')[1].scan(/\d+/)
			if pos != nil
				arduino_buffer[Time.now.iso8601] = a.split('|')[1].scan(/\d+/)[0]
			end
		end

		if a.include?('play')
			state = 'sync'
			pos = a.split('|')[1].scan(/\d+/)
			if pos != nil
				arduino_buffer[Time.now.iso8601] = a.split('|')[1].scan(/\d+/)[0]
			end
		end

		if state == 'sync'
			sp.write('sync ' + getLast(client_buffer).to_s())
		end
	end
}

def getLast(hash)
	if hash.length <1 
		return 0
	end
	return hash.sort_by{|time, value| time}[-1][1]
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
	if pos >=0 and pos <=1
		client_buffer[Time.now.iso8601] = pos
	else 
		"Error: pos is out of range"
	end
end


