require 'sinatra'

Tilt.register Tilt::ERBTemplate, 'html.erb'

def herb(template, options={}, locals={})
  render "html.erb", template, options, locals
end

get '/' do 
	herb :index
end
