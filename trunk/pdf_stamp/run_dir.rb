# first extract first page and change pdf size
# add stamp
# concatenate
require 'fileutils'

def mkdir_if_not_exist(path); FileUtils.mkdir_p(path) unless File.exist?(path); end

root_dir = 'C:/Users/ferryzhou/Documents/My Dropbox/LVLVLVKA/exhibit_material_sorted/'
out_root = '../out'; mkdir_if_not_exist(out_root)

$left_shift = "0"
 
#exhibit_ind = "A 3.4.1"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.1 (05 ICME) (3)')

#exhibit_ind = "A 3.4.2"; $exhibit_len = "0.10"; $left_shift = "0.01"
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.2 (06 ICRA) (12)')

#exhibit_ind = "A 3.4.3"; $exhibit_len = "0.10"; $left_shift = "0.01"
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.3 (06 ICIP) (12)')

#exhibit_ind = "A 3.4.4"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.4 (06 ICPR) (1)')

#exhibit_ind = "A 3.4.5"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.5 (07 ICASSP) (3)')

#exhibit_ind = "A 3.4.6"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.6 (07 ISVC) (1)')

#exhibit_ind = "A 3.4.7"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.7 (08 CVPR) (5)')

#exhibit_ind = "A 3.4.8"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.8 (08 JOSA) (1)')

#exhibit_ind = "A 3.4.9"; $exhibit_len = "0.09";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.9 (09 TSVCT) (4)')

#exhibit_ind = "A 3.4.10"; $exhibit_len = "0.11"; $left_shift = "0.03"
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.10 (02 keystroke) (15)')

#exhibit_ind = "A 3.4.11"; $exhibit_len = "0.11"; $left_shift = "0.03"
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.11 (04 xml) (21)')

#exhibit_ind = "A 3.4.12"; $exhibit_len = "0.11"; $left_shift = "0.03"
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.12 (04 intelligence) (10)')

#exhibit_ind = "A 3.4.13"; $exhibit_len = "0.10"; $left_shift = "0.02"
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)', 'A3.4.13 (05 tmn) (2)')

#exhibit_ind = "A 1"; $exhibit_len = "0.07";
#src_dir = File.join(root_dir, '(A1) publications')

#exhibit_ind = "A 3.1"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.1 (google scholar)')

#exhibit_ind = "A 3.2"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.2 (cnki citation)')

#exhibit_ind = "A 3.3"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.3 (wanfang citation)')

#exhibit_ind = "A 3.4"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A3) citations', 'A3.4 (citations)')

#exhibit_ind = "A 3"; $exhibit_len = "0.06";
#src_dir = File.join(root_dir, '(A3) citations')

#exhibit_ind = "A 4"; $exhibit_len = "0.06";
#src_dir = File.join(root_dir, '(A4) Introduction of Selected Journals and Conferences)')

#exhibit_ind = "A 2.1"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A2) patents', 'A2.1 (patent 1)')

#exhibit_ind = "A 2.2"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A2) patents', 'A2.2 (patent 2)')

#exhibit_ind = "A 2.3"; $exhibit_len = "0.08";
#src_dir = File.join(root_dir, '(A2) patents', 'A2.3 (invention disclosures)')

exhibit_ind = "B 1"; $exhibit_len = "0.06";
src_dir = File.join(root_dir, '(B1) reference letters')

#exhibit_ind = "B 2"; $exhibit_len = "0.06";
#src_dir = File.join(root_dir, '(B2) E-How')

#exhibit_ind = "B 3"; $exhibit_len = "0.06";
#src_dir = File.join(root_dir, '(B3) projects')

#exhibit_ind = "B 4"; $exhibit_len = "0.06";
#src_dir = File.join(root_dir, '(B4) journal review invitation')

#exhibit_ind = "1"; $exhibit_len = "0.03";
#src_dir = File.join(root_dir, '(1) CV')

#exhibit_ind = "2"; $exhibit_len = "0.03";
#src_dir = File.join(root_dir, '(2) offer letter')

#exhibit_ind = "Appendix"; $exhibit_len = "0.12"; $left_shift = "0.03"
#src_dir = File.join(root_dir, 'appendix')


out_dir = File.join(out_root, exhibit_ind); mkdir_if_not_exist(out_dir)
letter_p1_dir = File.join(out_dir, 'letter_p1'); mkdir_if_not_exist(letter_p1_dir)
stamp_dir = File.join(out_dir, 'stamp_dir'); mkdir_if_not_exist(stamp_dir)

def changeSizeToLetter(from, to)
	str = "\"C:/Program Files (x86)/Java/jdk1.6.0_20/bin/java\" -cp \"iText-5.0.5.jar;.\" ChangeSizeToLetter "
	str = str + "\"#{from}\" \"#{to}\" "
    puts str
    system(str) 
end

def addStamp(from, to, index)
    str = "\"C:/Program Files (x86)/Java/jdk1.6.0_20/bin/java\" -cp \"iText-5.0.5.jar;.\" StampText "
	str = str + "\"#{from}\" \"#{to}\" "
	str = str + "\"#{index}\" #{$exhibit_len} #{$left_shift}"
    puts str
    system(str) 
end

def concatenate(dir, out)
	str = "pdftk.exe \"#{File.join(dir, '*.pdf')}\" cat output \"#{out}\""
    puts str
    system(str) 	
end

files = Dir.glob(File.join(src_dir, "*.pdf"))
i = 1
files.each { |file| 
	basename = File.basename(file)
	letter_p1_file = File.join(letter_p1_dir, basename)
	changeSizeToLetter(file, letter_p1_file)
	ind = exhibit_ind
	if ind == 'Appendix'; ind = ind + " #{i}"
	else ind = ind + ".#{i}" if files.size > 1; end
	addStamp(letter_p1_file, File.join(stamp_dir, basename), ind)
	i = i + 1
}
concatenate(stamp_dir, File.join(out_root, exhibit_ind + '.pdf'))
