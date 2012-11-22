#include "precompiled.h"
#include "test_config.hpp"
#include <cstdlib>
#include <stdexcept>

namespace fs = boost::filesystem;

boost::shared_ptr<test_config> test_config::instance_;

test_config::test_config()
{
  declare_paths();
}

test_config::~test_config() {}

bool test_config::initialize()
{
  if(!instance_)
      instance_ = boost::make_shared<test_config>();
  return true;
}

void test_config::terminate()
{
  instance_.reset();
}

void test_config::declare_paths()
{
  fs::path data_path;
  if(!resolve_test_data_path(data_path))
      throw std::runtime_error("Cannot resolve TESTING_DATA_DIR.");
  paths_.insert(path_map_val( "Data", data_path));

  fs::path out_path;
  if(!resolve_test_output_path(out_path))
      throw std::runtime_error("Cannot resolve TESTING_OUTPUT_DIR.");
  paths_.insert(path_map_val( "Output", out_path));
}

bool test_config::resolve_test_data_path(fs::path& path)
{
  const char* const val = std::getenv("TESTING_DATA_DIR");
  if((!val) || (!*val))
  {
      path.clear();
      return false;
  }

  path = fs::path(val);
  return true;
}

bool test_config::resolve_test_output_path(fs::path& path)
{
  const char* const val = std::getenv("TESTING_OUTPUT_DIR");
  if((!val) || (!*val))
  {
      path.clear();
      return false;
  }

  path = fs::path(val);
  return true;
}

bool test_config::get_dir_path(const std::string& dir_key,
                               fs::path& full_path)
{
  const path_map_cit i(paths_.find(dir_key));
  if(i == paths_.end())
      return false;

  full_path = i->second;

  if(!fs::exists(full_path))
      fs::create_directories(full_path);

  return true;
}

bool test_config::get_dir_path(const std::string& dir_key,
                               const std::string& folder,
                               fs::path& full_path)
{
  fs::path dir_path;
  if(!get_dir_path(dir_key, dir_path))
      return false;

  full_path = dir_path / folder;

  if(!fs::exists(full_path))
      fs::create_directories(full_path);

  return true;
}

bool test_config::get_file_path(const std::string& dir_key,
                                const std::string& file_name,
                                fs::path& full_path)
{
  fs::path dir_path;
  if(!get_dir_path(dir_key, dir_path))
      return false;

  full_path = dir_path / file_name;

  return fs::exists(full_path);
}

bool test_config::get_file_path(const std::string& dir_key,
                                const std::string& file_name,
                                std::string& full_path)
{
    fs::path full_path_;
    bool status = get_file_path(dir_key, file_name, full_path_);
    full_path = full_path_.string();
    return status;
}

bool test_config::get_file_path(const std::string& dir_key,
                                const std::string& file_name,
                                std::wstring& full_path)
{
    fs::path full_path_;
    bool status = get_file_path(dir_key, file_name, full_path_);
    full_path = full_path_.wstring();
    return status;
}
