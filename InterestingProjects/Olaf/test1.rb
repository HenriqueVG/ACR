require 'fileutils'
require 'tempfile'
require 'tmpdir'


class Match
  attr_accessor :result_file_name, :modification, :parameter, :score, :query_file, :ref_file_start, :time_diff, :match_expected

  def hash_key
    "#{modification}_#{parameter}"
  end

  def file_names_match?
    q_basename = File.basename(query_file)
    #check if query and result names match
    q_basename =~ /(.*)_\d+s-\d+s.*/
    result_part = $1
    result_file_name =~ /.*#{result_part}.*/
  end

  def true_positive
    (match_expected && score > 0 && file_names_match?) ? 1 : 0
  end

  def true_negative
    (!match_expected && score == 0 )? 1 : 0
  end

  def false_postitive
    (!match_expected && score > 0  ) ? 1 : 0
  end

  def false_negative
    (match_expected && score == 0 )? 1 : 0
  end

end


query_file = "/home/gomesh/Documents/ACR/Dataset/fma_small/000/000002.mp3"
target_file = "/home/gomesh/Documents/ACR/InterestingProjects/Olaf/test_file.mp3"

command = "sox \"#{query_file}\" \"#{target_file}\" chorus 0.7 0.9 55 0.4 0.25 2 -t"
system(command)



lines = `olaf query #{query_file} 2>/dev/null`.split("\n")

  lines.shift #remove header
  line = lines.first

  #match count (#), q start (s) , q stop (s), ref path, ref ID, ref start (s), ref stop (s)
  #store only first match
indx,count,query_file,score ,query_start,query_stop,ref_path,ref_id,ref_start,ref_stop = line.split(",").map(&:strip)
  m = Match.new

  m.score = score.to_i
  m.result_file_name = ref_path
  m.modification = modification
  m.parameter = parameter
  m.ref_file_start = ref_start.to_f
  m.time_diff = ref_start.to_f - query_start.to_f
  m.match_expected = ("tp"==tp_or_tn_expected)
  m.query_file = query_file

  #puts "line: #{line} "
  puts "match: #{m.file_names_match?} #{m.query_file} #{m.result_file_name}"

