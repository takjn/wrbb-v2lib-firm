@value = 6

# 1000ms毎に行う処理の定義
Timer.set(1000) do
  @value -= 1
  puts "timer:#{@value}"
end

# タイマースタート
Timer.start

while true do
  led
  delay 100

  break if @value < 1
end

# タイマー停止
Timer.stop