#include <napi.h>
#include "processor.hpp"

static Napi::Value ProcessBuffer(const Napi::CallbackInfo& info) {
    auto env{info.Env()};

    if (info.Length() < 2 || !info[0].IsBuffer() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected (Buffer, Number)").ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto buffer{info[0].As<Napi::Buffer<std::uint8_t>>()};
    const auto xor_key{static_cast<std::uint8_t>(info[1].As<Napi::Number>().Uint32Value())};

    const auto* buffer_ptr{buffer.Data()};
    const auto buffer_length{static_cast<std::uint8_t>(buffer.Length())};

    auto [data, checksum] {
        CoreEngine::BufferEncrypter::convert(buffer_ptr, buffer_length, xor_key)
    };

    const auto output{Napi::Buffer<uint8_t>::New(env,data.size())};
    std::memcpy(output.Data(), data.data(), data.size());

    const auto response{Napi::Object::New(env)};
    response.Set("data", output);
    response.Set("checksum", Napi::Number::New(env, checksum));

    return response;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("processBuffer", Napi::Function::New(env, ProcessBuffer));
    return exports;
}

NODE_API_MODULE(native_module, Init);