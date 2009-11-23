#!/usr/bin/ruby
class MountPoint
	$barFill  = "@"
	$barSpace = "-"
	attr_accessor :mp, :parsent
	def initialize(name,parsent)
		@mp = name.to_s
		@parsent = parsent.to_i
	end
	def to_line(labelLen,barLen)
		return "#{self.mp.ljust(labelLen)} #{($barFill*(self.parsent.to_f*barLen/100).to_i).ljust(barLen,$barSpace)} #{self.parsent.to_s.rjust(3)}%\n"
	end
end

class DfGraph
	$defaultWidth = 60
	def DfGraph.start
		df = `df`.split("\n").select{|x|x.match(/\/\S*/)}
		datun = df.map{|x|
			t=x.split(" ")
			MountPoint.new(t.select{|x|x.match(/\/\S*/)}.last , t.select{|x|x.match(/\d+\%/)}.first)
		}
		labelLen = datun.map{|x|x.mp.length}.max.to_i
	
		`getScreenWH 2> /dev/null`.match(/(\d+)x\d+/)
		barLength = if $1.to_i>0 then $1.to_i-labelLen-7 else $defaultWidth end
	
		datun.each{|x|puts x.to_line(labelLen,barLength)}
	end
end

DfGraph.start


