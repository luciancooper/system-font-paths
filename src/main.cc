#include <napi.h>
#include <stdlib.h>

#include <memory>
#include <string>
#include <thread>

#include "collection.h"

// this function is implemented depending on the platform
StringVector* getSystemFonts();

Napi::Array fontListToNapiArray(Napi::Env env, StringVector& results) {
    Napi::Array res = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); i++) {
        res.Set(i, Napi::String::New(env, results[i]));
    }
    return res;
}

class SystemFontsWorker final : public Napi::AsyncWorker {
  private:
    std::unique_ptr<StringVector> rs_;

  public:
    explicit SystemFontsWorker(Napi::Function& callback) : Napi::AsyncWorker(callback) {}

    ~SystemFontsWorker() = default;

    void Execute() override { rs_.reset(getSystemFonts()); }

    void OnOK() override {
        Napi::HandleScope scope(Env());
        Napi::Array result = fontListToNapiArray(Env(), *rs_.release());
        Callback().Call({result});
    }
};

Napi::Value systemFontPathsWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Function cb = info[0].As<Napi::Function>();
    auto* wk = new SystemFontsWorker(cb);
    wk->Queue();
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("systemFontPaths", Napi::Function::New(env, systemFontPathsWrapped));
    return exports;
}

NODE_API_MODULE(systemfontpaths, Init)
