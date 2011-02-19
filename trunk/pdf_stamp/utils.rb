require 'fileutils'

def mkdir_if_not_exist(path); FileUtils.mkdir_p(path) unless File.exist?(path); end