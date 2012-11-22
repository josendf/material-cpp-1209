#pragma once
#ifndef TEST_CONFIG_HPP_
#define TEST_CONFIG_HPP_
#include <map>
#include <string>
#include <ext/boost.hpp>
#include <ext/boost/filesystem.hpp>
#include <ext/boost/date_time.hpp>
#include <ext/boost/test.hpp>

class test_config {
public:
  typedef boost::shared_ptr<test_config> shared_ptr;

  typedef boost::filesystem::path path_type;

  typedef std::map<std::string, path_type> path_map;

  typedef path_map::const_iterator path_map_cit;

  typedef path_map::value_type path_map_val;

  typedef path_map::iterator path_map_it;

private:
  static boost::shared_ptr<test_config> instance_;

  path_map paths_;

public:
  test_config();

  ~test_config();

  static bool initialize();

  static void terminate();

  static shared_ptr get() {
      return instance_;
  }

  bool get_dir_path(const std::string& dir_key,
                    path_type& full_path);

  bool get_dir_path(const std::string& dir_key,
                    const std::string& folder,
                    path_type& full_path);

  bool get_file_path(const std::string& dir_key,
                     const std::string& file_name,
                     path_type& full_path);

  bool get_file_path(const std::string& dir_key,
                     const std::string& file_name,
                     std::string& full_path);

  bool get_file_path(const std::string& dir_key,
                     const std::string& file_name,
                     std::wstring& full_path);

private:
  void declare_paths();

  bool resolve_test_data_path(path_type& path);

  bool resolve_test_output_path(path_type& path);
};


#endif
