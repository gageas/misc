class MSF
	@frames
	def initialize frames
		@frames = frames.to_i
	end
	def doit
		p @frames
	end
	def toMSF
		[self.getMin,self.getSec,self.getFrame]
	end
	def to_s
		"#{self.getMin}:#{self.getSec}.#{self.getFrame}"
	end
	def getMin
		(@frames/75/60)
	end
	def getSec
		(@frames/75) % 60
	end
	def getFrame
		@frames % 75
	end
end

test = MSF.new 1024
p test.toMSF

