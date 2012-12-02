require 'rubygems'
require 'serialport' 

sp = SerialPort.new "/dev/ttyACM0", 9600
sp.write('hi')

while 1
	
	a = ''	
	b = '1'
	while (b != '-')
		b = sp.getc
		if (b!=nil)
			a += b
		end
	end

	if (a != nil)
		puts 'a = ' +  a
		puts 'state = '+ a[/\D+(?:\D+)/]
		puts 'pos = ' + a[/\d+(?:\.\d+)?/]
	end
end
