#!ruby -Ku
require 'digest/md5'

unless ENV['OS'].nil? then # OSがWindowsであるとき、putsとSTDERR.putsをsjis出力対応にオーバーライド
	require 'kconv'
	def puts str
		print(Kconv.tosjis(str+"\n"))
	end
	def STDERR.puts str
		STDERR.print(Kconv.tosjis(str+"\n"))
	end
end

# 例外を吐くことがあるのに注意
class Digest::Base
	def self.open(path)
		obj = new
		File.open(path, 'rb') { |f|
			buf = ""
			while f.read(256, buf)
				obj << buf
			end
		}
		obj
	end
end


class UniqFile
	attr_accessor :name,:size
	def initialize (name,size)
		@name = name
		@size = size
	end
end


class Unique
	def initialize args
		@options = {
			'exec' => false,
			'long' => false,
			'recursive' => false,
			'recurse_dir_first' => false
		}
		@options.merge!(args)
		
		@files_by_size = Hash.new([].freeze) # ref. http://www.ruby-lang.org/ja/man/html/trap_Hash.html
	end

	# ファイルサイズが同じ複数のファイルについて、md5sumを見て比較する
	# 同じファイルサイズのものがあれば削除する
	def makeUnique
		@files_by_size.each_pair{|k,v|
			next if v.length <=1 # あるファイルサイズのファイルが1つだけの時は何もしない
			hash = {}
			filename_primary = {}
			
			v.each{|fname|
				begin
					md5 = Digest::MD5.open(fname).hexdigest
				rescue
					next
				end
				
				if(md5.nil?) then next end
				if(hash[md5] == true)&&(fname != filename_primary[md5]) then
					keepfname = filename_primary[md5]
					puts "Delete : #{fname}\n"+
						 "Keep   : #{keepfname}"
					if @options["exec"] then
						File.open(keepfname){|keep|
							begin
								keep.flock(File::LOCK_SH) # shared lock. throw Errno::EBADF
								File.delete(fname) # throw Errno::EACCES
								keep.flock(File::LOCK_UN) # unlock
							rescue Errno::EBADF
								STDERR.puts "!!! ERROR !!! Can't lock original(keep) file \"#{keepfname}\"."
							rescue Errno::EACCES
								STDERR.puts "!!! ERROR !!! Can't delete file \"#{fname}\"."
							end
						}
						
					end
				else
					hash[md5] = true
					filename_primary[md5] = fname
					puts (md5 + " " + fname) if @options["long"]
				end
			}
		}
	end
	
	# path内のファイルを（再帰的に）探して@files_by_sizeにつっこんでいく
	def enumFiles(path)
		files = []
		dirs = []
		
		def procSubDir dirs
			dirs.each{|subdir| self.enumFiles(subdir)}
		end
		
		begin
			imgs = Dir.open(path).sort{|x,y|x.length<=>y.length}
		rescue
			return
		end
		imgs.each{|x|
			next if ((x == '.') | (x == '..'))
			fname = path + "/" + x
			if (File.directory?(fname)) then
				dirs.push(fname)
				next
			end
			
			if (File.file?(fname)) then
				size = File.size?(fname)
				files.push(UniqFile.new(fname,size))
				next
			end
		}
		
		procSubDir dirs if (@options["recursive"] && @options["recurse_dir_first"])
		files.each{|ufile|
			@files_by_size[ufile.size] += [ufile.name]
		}
		procSubDir dirs if (@options["recursive"] && !@options["recurse_dir_first"])
	end
end

param = {}

paths = ARGV.select{|x| x[0..0] != "-"}.map{|x| x.sub(/\/$/,"")}.uniq
# parse arguments
ARGV.each{|arg|
	case arg
		when "--exec"
			param["exec"] = true
		when "--long"
			param["long"] = true
		when "--recursive"
			param["recursive"] = true
		when "--recurse-dir-first"
			param["recurse_dir_first"] = true
		when /-([^-]+)/
			$1.split("").each{|c|
				case c
					when "l"
						param["long"] = true
					when "x"
						param["exec"] = true
					when "r"
						param["recursive"] = true
					when "d"
						param["recursive"] = true
						param["recurse_dir_first"] = true
				end
			}
	end
}
if(paths.length == 0) then puts "$ unique.rb path [path2...] [-x] [-l] [-r]";exit end
STDERR.puts "["+paths.map{|x|'"'+x+'"'}.join(" , ")+"]"
STDERR.puts(if(param["exec"]) then "exec mode" else "test mode" end)

unique = Unique.new(param)
STDERR.puts "ファイルリストを作成しています"
paths.each{|path|unique.enumFiles path}
STDERR.puts "重複ファイルの検出中"
unique.makeUnique
