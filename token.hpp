/* Authors: Tyler Burkett
 * Part of the handout for project 1 of CS441 Fall 2024
 * Description: Token types the scanner will produce when scanning a file.
 */
#pragma once
#include <string>
#include <variant>
#include <optional>

// Structures for all tokens
template<class T, int id = 0>
struct base_token { 
    using value_ty = T; 

    std::pair<int, int> start;
    std::pair<int, int> end;
    T val;
};

using int_token =      base_token<int>;
using float_token =    base_token<float>;
using string_token =   base_token<std::string>;
using char_token =     base_token<char>;
using bool_token =     base_token<bool>;
using id_token =       base_token<std::string, 1>;
using keyword_token =  base_token<std::string, 2>;
using symbol_token =   base_token<std::string, 3>;
using error_token =    base_token<std::string, 4>;
using comment_token =  base_token<std::string, 5>;
using end_of_input =   struct empty_token {};

// Variant type containing all tokens 
using token_ty = std::variant<end_of_input, int_token, float_token, string_token, char_token, 
                              bool_token, id_token, keyword_token, symbol_token, error_token,
                              comment_token>;

// Determine if a variant *V* holds an object of type *T*
template <class T, class V>
bool match(const V& v) {
    return std::holds_alternative<T>(v);
}

// Determine if a variant *V* holds an object of type *T* with 'val' filed containg object *c*
template<class T, class V, class C>
bool match(const V& v, const C&& c) {
    if constexpr (std::is_same<typename T::value_ty, C>::value) {
        return std::holds_alternative<T>(v) and std::get<T>(v).val == c;
    }
    else {
        return false;
    }
}

// If *V* contains an object *T*, return that object
template <class T, class V>
std::optional<T> expect(V& v) {
    if (match<T>(v)) {
        std::optional<T> result = v.template get<T>();
        return result;
    }
    else {
        return std::nullopt;
    }
}

// If *V* contains an object *T* with 'val' filed holding *c*, return that object
template<class T, class V, class C>
std::optional<T> expect(V& v, const C&& c) {
    if (match<T>(v, c)) {
        std::optional<T> result = v.template get<T>();
        return result;
    }
    else {
        return std::nullopt;
    }
}
