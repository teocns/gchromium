#include "fingerprinting/core/evasions/hook.h"
namespace fingerprinting::core::evasions {

// std::string Hook::get_definition() {
//   /*
//    * Geneerates the JavaScript definition of the "hook" function
//    * A random name will be generated for
//    */
//   return std::format("{} = function(){{ {} }};", this->codename()), this->get_impl());
// }

std::string Hook::get_iife() {
#ifdef NDEBUG
  return std::format("(function(){{ {} }})()", get_impl(target));
#else
  return std::format("(function(){{ console.info('Running hook {}'); {} }})()", codename(), get_impl());
#endif
}


}  // namespace fingerprinting::core::evasions
