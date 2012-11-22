/**
 * @file   types.hpp
 * @author jedf
 *
 * @brief  htparse parsing types
 */
#pragma once
#ifndef HTPARSE_TYPES_HPP_
#define HTPARSE_TYPES_HPP_

namespace htparse {

// Parsing
typedef htparse::basic_message<context> message;
typedef htparse::basic_parser<message> parser;
typedef htparse::basic_expr<message> expr;
typedef htparse::basic_filter<message>::filter_func filter;
typedef basic_evaluator<context, message, parser> evaluator;

// Filters
typedef htparse::filters::method<message> method;
typedef htparse::filters::params<message> params;
typedef htparse::filters::output<message> output;
typedef htparse::filters::choice<message> choice;
typedef htparse::filters::header<message> header;
typedef htparse::filters::authorization<message> authorization;
typedef htparse::filters::mac_auth<message> mac_auth;
typedef htparse::filters::option<message> option;
typedef htparse::filters::path<message> path;
typedef htparse::filters::content<message> content;
typedef htparse::filters::rheader<message> rheader;
typedef htparse::filters::no_cache<message> no_cache;
typedef htparse::filters::on_error<message> on_error;
typedef htparse::filters::on_error_complete<message> on_error_complete;

}
#endif
