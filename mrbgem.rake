MRuby::Gem::Specification.new('mruby-WiringPi') do |spec|
  spec.license = 'LGPL'
  spec.authors = 'Akira Yumiyama <yumiyama.akira@gmail.com>'

  spec.cc.include_paths << "#{spec.dir}/include" << "#{spec.dir}/wiringPi"

  spec.objs << (Dir.glob("#{dir}/src/*.c") + Dir.glob("#{dir}/wiringPi/*.c")).map { |f| f.relative_path_from(dir).pathmap("#{build_dir}/%X.o") }
end
