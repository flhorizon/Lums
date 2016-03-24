#include <LumsInclude/Provider_internal.hpp>

#include <LumsInclude/Singleton_internal_inline.hpp>

// Specialized Providers from Provider.hpp
LM_I_FORCE_SINGLETON_INSTANCE(ShaderProvider, lm::ShaderProvider)
LM_I_FORCE_SINGLETON_INSTANCE(ImageProvider, lm::ImageProvider)
LM_I_FORCE_SINGLETON_INSTANCE(TextureProvider, lm::TextureProvider)
LM_I_FORCE_SINGLETON_INSTANCE(FontProvider, lm::FontProvider)
LM_I_FORCE_SINGLETON_INSTANCE(GameObjectProvider, lm::GameObjectProvider)