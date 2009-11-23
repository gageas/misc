#!/usr/bin/env ruby
#
#
# 画像リサイズするよ
#-----------------------
#      usage
#      $ ruby resize.rb 入力ファイル名 [出力ファイル名] [width] [height]
#        入力のファイルを携帯壁紙サイズに変換して出力
# 
#require 'RMagick'


class ImageMagicWrap
	def self.getResolutionFromFile(filename)
		ret  = `identify #{filename}`.gsub(/(\d+)x(\d+)/,"")
#s/(\d+)x(\d+)//g; #ファイル名にじおめとりっぽいのがあってもこれなら大丈夫っぽい
		width=$1.to_f
		height=$2.to_f
		if width == 0 && height == 0 then return nil end
		{:width => width, :height => height}
	end
	def self.resizeFile(filename,destfile,destwidth,destheight,format=nil)
		dest_w = destwidth.to_f
		dest_h = destheight.to_f
		if (dest_w == 0) && (dest_h == 0) then return end
		src_resolution = getResolutionFromFile filename
		return if src_resolution.nil?
		width = src_resolution[:width]
		height = src_resolution[:height]
		cropgeo = dest_w.to_s+"x"+ dest_h.to_s
		if (dest_w == 0) || (dest_h ==0) then
			if dest_w == 0 then
				geometry = "x#{dest_h}"
			end
			if dest_h == 0 then
				geometry = "#{dest_w}x"
			end
		else
			if ((width/dest_w)>(height/dest_h)) then
		  		geometry = "x#{dest_h}"
		  		tmp = (((width/height*dest_h) - dest_w)/2).to_i
		  		cropgeo = "#{dest_w}x#{dest_h}+#{tmp}"
			else
			    geometry = "#{dest_w}x"
			    tmp = (((height/width*dest_w) - dest_h)/2).to_i
			    cropgeo = "#{dest_w}x#{dest_h}+0+#{tmp}"
			end
		end
		command = "convert -resize #{geometry} -crop #{cropgeo}  \"#{filename}\" #{unless format.nil? then format+':' else '' end}\"#{destfile}\""
		system(command)
	end
end

format = nil
dwidth  = 240
dheight = 320

case ARGV.length
  when 5
    filename = ARGV[0]
    output = ARGV[1]
    format = ARGV[2]
    dwidth = ARGV[3]
    dheight = ARGV[4]
  when 4
    filename = ARGV[0]
    output = ARGV[1]
    dwidth = ARGV[2]
    dheight = ARGV[3]
  when 3
    filename = ARGV[0]
    output = ARGV[1]
    (dwidth,dheight) = ARGV[2].split('x')
  when 2
    filename = ARGV[0]
    output = ARGV[1]
  when 1
    filename = ARGV[0]
    output = ARGV[0]+".png"
  else
    STDERR.print "\n\n   USAGE : \n\n"
    STDERR.print "     ruby resize.rb [input] \n"
    STDERR.print "      (output filename is [input].png)\n"
    STDERR.print "     ruby resize.rb [input] [output] \n"
	STDERR.print "     ruby resize.rb [input] [output] [widthXheight]\n"
	STDERR.print "     ruby resize.rb [input] [output] [width] [height]\n"
end

ImageMagicWrap.resizeFile(filename,output,dwidth,dheight,format) if ARGV.length > 0
