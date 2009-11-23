#!/usr/bin/ruby
require 'thread'

N = if ARGV[0] then ARGV[0].to_i else 2 end # Number of worker processes

tasks = Queue.new
STDIN.each{|task|tasks.push task}
puts "process #{tasks.length} tasks in #{N} workers"

worker = Proc.new do |i|
	Thread.pass
	until tasks.empty? do
		task = tasks.shift
		if task.nil? then next end
		puts "processing #{task} by worker##{i}"
		`#{task}`
	end
end

threads = [];
N.times do |i|
	th = Thread.start(i) do |i|
		worker.call(i)
	end
	threads.push th
end

threads.each{|th|th.join}
