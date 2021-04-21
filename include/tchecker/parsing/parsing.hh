/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_PARSING_HH
#define TCHECKER_PARSING_HH

#include <cstdio>
#include <string>

#include "tchecker/expression/expression.hh"
#include "tchecker/parsing/declaration.hh"
#include "tchecker/statement/statement.hh"
#include "tchecker/utils/log.hh"

namespace tchecker {
  
  namespace parsing {
    
    /*!
     \brief Parser for systems
     \param filename : file to parse
     \param log : logging facility
     \return The system declaration read from filename, nullptr if parsing failed
     \post All errors and warnings have been reported on log
     */
    tchecker::parsing::system_declaration_t * parse_system_declaration(std::string const & filename, tchecker::log_t & log);
    
    
    /*!
     \brief Parser for systems
     \param f : file to parse
     \param filename : name of file f
     \param log : logging facility
     \return The system declaration read from f, nullptr if parsing failed
     \post All errors and warnings have been reported on log
     \note filename is used as context for error/warning messages
     */
    tchecker::parsing::system_declaration_t *
    parse_system_declaration(std::FILE * f, std::string const & filename, tchecker::log_t & log);
    
    
    /*!
     \brief Parser for expression
     \param expr_context : context (location in file, etc) for string to parse
     \param expr_str : string to parse
     \param log : logging facility
     \return The expression read from expr_str, nullptr if parsing failed
     \post All errors and warnings have been reported on log
     \note expr_context is used in error messages to provide context
     information regarding expr_str. It is ignored if empty
     */
    tchecker::expression_t * parse_expression(std::string const & expr_context, std::string const & expr_str,
                                              tchecker::log_t & log);
    
    
    /*!
     \brief Parser for statements
     \param stmt_context : context (location in file, etc) for string to parse
     \param stmt_str : string to parse
     \param log : logging facility
     \return The statement read from stmt_str, nullptr if parsing failed
     \post All errors and warnings have been reported on log
     \note stmt_context is used in error messages to provide context
     information regarding stmt_str. It is ignored if empty
     */
    tchecker::statement_t * parse_statement(std::string const & stmt_context, std::string const & stmt_str, tchecker::log_t & log);
    
    
    
    
    /*!
     \class attributes_parser_t
     \brief Interface of a parser of declaration attributes
     */
    class attributes_parser_t {
    public:
      /*!
       \brief Constructor
       */
      attributes_parser_t() = default;
      
      /*!
       \brief Copy constructor
       */
      attributes_parser_t(tchecker::parsing::attributes_parser_t const &) = default;
      
      /*!
       \brief Move constructor
       */
      attributes_parser_t(tchecker::parsing::attributes_parser_t &&) = default;
      
      /*!
       \brief Destructor
       */
      ~attributes_parser_t() = default;
      
      /*!
       \brief Assignment operator
       */
      tchecker::parsing::attributes_parser_t & operator= (tchecker::parsing::attributes_parser_t const &) = default;
      
      /*!
       \brief Move assignment operator
       */
      tchecker::parsing::attributes_parser_t & operator= (tchecker::parsing::attributes_parser_t &&) = default;
      
      /*!
       \brief Attributes parser
       \param attributes : attributes
       \post do_attr() has been called on every attribute in attributes
       */
      inline void parse(tchecker::parsing::attributes_t const & attributes)
      {
        for (tchecker::parsing::attr_t const & attr : attributes)
          do_attr(attr);
      }
    protected:
      /*!
       \brief Attribute parser
       \param attr : attribute
       */
      virtual void do_attr(tchecker::parsing::attr_t const & attr) = 0;
    };

    /*!
      \class Stack Operation Parser
      \brief Interface of a parser for stack operations
    */
    class stack_op_parser_t{
    public:
      /*!
       \brief Constructor
      */
      stack_op_parser_t() = default;

      /*!
       \brief Copy constructor
      */
      stack_op_parser_t(tchecker::parsing::stack_op_parser_t const &) =  default;

      /*!
       \brief Move constructor
       */
      stack_op_parser_t(tchecker::parsing::stack_op_parser_t  &&) = default;

      /*!
       \brief Destructor
      */
      ~stack_op_parser_t() = default;

      /*!
       \brief Assignment operator
      */
      tchecker::parsing::stack_op_parser_t & operator= (tchecker::parsing::stack_op_parser_t const &) = default;

      /*!
       \brief Move assignment operator
      */
      tchecker::parsing::stack_op_parser_t & operator= (tchecker::parsing::stack_op_parser_t &&) = default;

      /*!
       \brief Stack Operation parser
       \param sop : Stack operation
       \post do_stack() has been called on sop.
      */
      inline void parse(tchecker::parsing::stack_op const & sop){
        do_stack(sop);
      }
    protected:
      /*!
       \brief Stack Operation parser
       \param sop : Stack Operation
      */
      virtual void do_stack(tchecker::parsing::stack_op const & sop) = 0;
    };
    
  } // namespace parsing
  
} // namespace tchecker


#endif // TCHECKER_PARSING_HH
