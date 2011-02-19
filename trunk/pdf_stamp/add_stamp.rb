require 'fileutils'

def mkdir_if_not_exist(path); FileUtils.mkdir_p(path) unless File.exist?(path); end

src_dir = "../publications/papers_letter_p1/"
dst_dir = "../publications/papers_letter_p1_stamped/"
stamp_base = "A.1"

mkdir_if_not_exist(dst_dir)

files = Dir.glob(src_dir + "*.pdf")
i = 1
files.each { |file| 
	basename = File.basename(file)
	out_file = dst_dir + basename
    str = "\"C:/Program Files (x86)/Java/jdk1.6.0_20/bin/java\" -cp \"iText-5.0.5.jar;.\" StampText "
	str = str + "\"#{file}\" \"#{out_file}\" "
	str = str + stamp_base + ".#{i}"
    puts str
    system(str) 
	i = i + 1
}