/**
 * @file   types.hpp
 * @author jedf
 * @date   Tue Aug 14 18:04:48 2012
 *
 * @brief  htparse CppCMS backend types
 */
#pragma once
#ifndef HTPARSE_BACKENDS_CPPCMS_TYPES_HPP_
#define HTPARSE_BACKENDS_CPPCMS_TYPES_HPP_

namespace htparse {

typedef ::cppcms::http::context raw_context;
typedef ::cppcms::http::request raw_request;
typedef ::cppcms::http::response raw_response;
typedef cppcms::basic_request<raw_request> request;
typedef cppcms::basic_response<raw_response> response;
typedef cppcms::basic_context<raw_context, request, response> context;
typedef context::context_ptr context_ptr;

}
#endif
