require 'fileutils'

def mkdir_if_not_exist(path); FileUtils.mkdir_p(path) unless File.exist?(path); end


src_dir = "../publications/papers_letter/"
dst_dir = "../publications/papers_letter_p1/"

mkdir_if_not_exist(dst_dir)

files = Dir.glob(src_dir + "*.pdf")
i = 1
files.each { |file| 
	basename = File.basename(file)
	out_file = dst_dir + basename
	if i <=18
		str = "\"C:/Program Files (x86)/GPLGS/gswin32c\" -sDEVICE=pdfwrite -dNOPAUSE -dBATCH -dSAFER -dFirstPage=1 -dLastPage=1 -sOutputFile=\"#{out_file}\" \"#{file}\""
	else
		str = "\"pdftk.exe\" \"#{file}\" cat 1 output  \"#{out_file}\""
	end
	i = i + 1
	puts str
	system(str) 
}