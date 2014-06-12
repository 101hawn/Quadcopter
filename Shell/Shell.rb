require 'socket'

begin
	input = TCPSocket.new('localhost',5558)
	output = TCPSocket.new('localhost',5555)
rescue 
	puts "error: #{$!}"
else
	@running = true
	the = Thread.new{
@gots = ''
while @running do
@gots = @gots + output.gets
end
}
	command = ''
	print ">>>"
	until command.include?('exit') do
	command = gets()
	input.puts(command)
	puts @gots
	@gots = "	"
	print ">>>"
	end
	@running = false

end
