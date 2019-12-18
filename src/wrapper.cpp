#include "wrapper.hpp"
#include <iostream>
using namespace std;

extern "C" {
#include "lib.h"
}

namespace adblock {

std::vector<FilterList> FilterList::default_list;
std::vector<FilterList> FilterList::regional_list;

FilterList::FilterList(const std::string& uuid,
                       const std::string& url,
                       const std::string& title,
                       const std::vector<std::string>& langs,
                       const std::string& support_url,
                       const std::string& component_id,
                       const std::string& base64_public_key)
    : uuid(uuid),
      url(url),
      title(title),
      langs(langs),
      support_url(support_url),
      component_id(component_id),
      base64_public_key(base64_public_key) {}

FilterList::FilterList(const FilterList& other) = default;

FilterList::~FilterList() {
}

// [static]
std::vector<FilterList>&  FilterList::GetDefaultLists() {
  return GetFilterLists("default");
}

// [static]
std::vector<FilterList>&  FilterList::GetRegionalLists() {
  return GetFilterLists("regions");
}

std::vector<FilterList>& FilterList::GetFilterLists(const std::string &category) {
  std::vector<FilterList>& list =
    category == "regions" ? regional_list : default_list;
  if (list.size() > 0) {
    return list;
  }

  size_t size = filter_list_size(category.c_str());
  for (size_t i = 0; i < size; i++) {
    std::vector<std::string> langs;
    C_FList l = filter_list_get(category.c_str(), i);
    if (strlen(l.lang) > 0)
      langs.push_back(l.lang);
    if (strlen(l.lang2) > 0)
      langs.push_back(l.lang2);
    if (strlen(l.lang3) > 0)
      langs.push_back(l.lang3);
    list.push_back(FilterList(l.uuid,
             l.url,
             l.title,
             langs,
             l.support_url,
             l.component_id,
             l.base64_public_key));
  }
  return list;
}

Engine::Engine() : raw(engine_create("")) {
}

Engine::Engine(const std::string& rules) : raw(engine_create(rules.c_str())) {
}

bool Engine::matches(const std::string& url, const std::string& host,
    const std::string& tab_host, bool is_third_party,
    const std::string& resource_type, bool* explicit_cancel,
    bool* saved_from_exception, std::string* redirect) {
  char* redirect_char_ptr = nullptr;
  bool result = engine_match(raw, url.c_str(), host.c_str(),tab_host.c_str(),
      is_third_party, resource_type.c_str(), explicit_cancel,
      saved_from_exception, &redirect_char_ptr);
  if (redirect_char_ptr) {
    if (redirect) {
      *redirect = redirect_char_ptr;
    }
    c_char_buffer_destroy(redirect_char_ptr);
  }
  return result;
}

bool Engine::deserialize(const char* data, size_t data_size) {
  return engine_deserialize(raw, data, data_size);
}

void Engine::addTag(const std::string& tag) {
  engine_add_tag(raw, tag.c_str());
}

void Engine::removeTag(const std::string& tag) {
  engine_remove_tag(raw, tag.c_str());
}

bool Engine::tagExists(const std::string& tag) {
  return engine_tag_exists(raw, tag.c_str());
}

void Engine::addFilter(const std::string& filter) {
  engine_add_filter(raw, filter.c_str());
}

void Engine::addResource(const std::string& key,
    const std::string& content_type,
    const std::string &data) {
  engine_add_resource(raw, key.c_str(), content_type.c_str(), data.c_str());
}

void Engine::addResources(const std::string& resources) {
  engine_add_resources(raw, resources.c_str());
}

Engine::~Engine() {
  engine_destroy(raw);
}

}  // namespace adblock
