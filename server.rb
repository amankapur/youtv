require 'rubygems'
require 'sinatra'
require 'serialport'
require 'thread'




sp = SerialPort.new "/dev/ttyACM0", 9600
sp.write('hi')
puts sp.read()





Tilt.register Tilt::ERBTemplate, 'html.erb'
def herb(template, options={}, locals={})
  render "html.erb", template, options, locals
end

get '/' do 
	herb :index
end


post '/sync' do 
	pos = params[:pos]
	len = params[:len]
	
end
