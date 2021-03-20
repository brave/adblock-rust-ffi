#ifndef ADBLOCK_RUST_FFI_SRC_WRAPPER_HPP_
#define ADBLOCK_RUST_FFI_SRC_WRAPPER_HPP_
#include <memory>
#include <string>
#include <vector>

extern "C" {
#include "lib.h"
}

#if defined(ADBLOCK_SHARED_LIBRARY)
#if defined(WIN32)
#if defined(ADBLOCK_IMPLEMENTATION)
#define ADBLOCK_EXPORT __declspec(dllexport)
#else
#define ADBLOCK_EXPORT __declspec(dllimport)
#endif  // defined(ADBLOCK_IMPLEMENTATION)
#else  // defined(WIN32)
#if defined(ADBLOCK_IMPLEMENTATION)
#define ADBLOCK_EXPORT __attribute__((visibility("default")))
#else
#define ADBLOCK_EXPORT
#endif  // defined(ADBLOCK_IMPLEMENTATION)
#endif
#else  // defined(ADBLOCK_SHARED_LIBRARY)
#define ADBLOCK_EXPORT
#endif

namespace adblock {

#ifndef EMBEDDED_DOMAIN_RESOLVER
typedef C_DomainResolverCallback DomainResolverCallback;

bool ADBLOCK_EXPORT SetDomainResolver(DomainResolverCallback resolver);
#endif

class ADBLOCK_EXPORT FilterList {
 public:
  FilterList(const std::string& uuid,
             const std::string& url,
             const std::string& title,
             const std::vector<std::string>& langs,
             const std::string& support_url,
             const std::string& component_id,
             const std::string& base64_public_key,
             const std::string& desc);
  FilterList(const FilterList& other);
  ~FilterList();

  const std::string uuid;
  const std::string url;
  const std::string title;
  const std::vector<std::string> langs;
  const std::string support_url;
  const std::string component_id;
  const std::string base64_public_key;
  const std::string desc;

private:
  static std::vector<FilterList> default_list;
  static std::vector<FilterList> regional_list;
};

class ADBLOCK_EXPORT Engine {
 public:
  Engine();
  Engine(const std::string& rules);
  void matches(const std::string& url, const std::string& host,
      const std::string& tab_host, bool is_third_party,
      const std::string& resource_type, bool* did_match_rule,
      bool* did_match_exception, bool* did_match_important,
      std::string* redirect);
  bool deserialize(const char* data, size_t data_size);
  void addTag(const std::string& tag);
  void addResource(const std::string& key,
      const std::string& content_type,
      const std::string& data);
  void addResources(const std::string& resources);
  void removeTag(const std::string& tag);
  bool tagExists(const std::string& tag);
  const std::string urlCosmeticResources(const std::string& url);
  const std::string hiddenClassIdSelectors(
      const std::vector<std::string>& classes,
      const std::vector<std::string>& ids,
      const std::vector<std::string>& exceptions
  );
  ~Engine();

 private:
  Engine(const Engine&) = delete;
  void operator=(const Engine&) = delete;
  C_Engine* raw;
};

}  // namespace adblock

#endif  // ADBLOCK_RUST_FFI_SRC_WRAPPER_HPP_
