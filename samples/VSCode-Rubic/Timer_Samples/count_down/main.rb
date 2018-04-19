@value = 10
@previous_value = 11

Timer.set(1000) do
  @value -= 1
end

Timer.start

while true do
  # led_test
  led 1
  delay 50
  led 0
  delay 50

  unless @value == @previous_value
    puts "timer:#{@value}"
    @previous_value = @value

    break if @value < 1
  end
end

Timer.stop