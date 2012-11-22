/**
 * @file   types.hpp
 * @author jedf
 * @date   Tue Aug 14 18:04:48 2012
 *
 * @brief  htparse testing backend types
 */
#pragma once
#ifndef HTPARSE_BACKENDS_TESTING_TYPES_HPP_
#define HTPARSE_BACKENDS_TESTING_TYPES_HPP_

namespace htparse {

typedef void* raw_context;
typedef std::string raw_request;
typedef std::stringstream raw_response;
typedef testing::basic_request<raw_request> request;
typedef testing::basic_response<raw_response> response;
typedef testing::basic_context<raw_context, request, response> context;
typedef context::context_ptr context_ptr;

}
#endif
