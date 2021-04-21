/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_GRAPH_BUILDER_HH
#define TCHECKER_GRAPH_BUILDER_HH

#include <tuple>
#include <type_traits>

#include "tchecker/basictypes.hh"

/*!
 \file builder.hh
 \brief Graph builder
 */

namespace tchecker {
  
  namespace graph {
    
    /*!
     \class ts_builder_t
     \brief Build a graph from a transition system
     \tparam TS : type of transition system (see tchecker::ts::ts_t)
     \tparam GRAPH : type of graph, should derive from tchecker::graph::graph_allocator_t.
     Should be garbage-collected
     */
    template <class TS, class GRAPH>
    class ts_builder_t {
      static_assert(std::is_base_of<typename TS::state_t, typename GRAPH::node_t>::value, "");
      static_assert(std::is_base_of<typename TS::transition_t, typename GRAPH::edge_t>::value, "");
    public:
      /*!
       \brief Type of pointer to node
       */
      using node_ptr_t = typename GRAPH::node_ptr_t;
      
      /*!
       \brief Type of pointer to edge
       */
      using edge_ptr_t = typename GRAPH::edge_ptr_t;
      
      /*!
       \brief Constructor
       \param ts : a transition system
       \param graph : a graph
       \note this keeps a reference on ts and a reference on graph
       */
      ts_builder_t(TS & ts, GRAPH & graph) : _ts(ts), _graph(graph)
      {}
      
      /*!
       \brief Copy constructor
       */
      ts_builder_t(tchecker::graph::ts_builder_t<TS, GRAPH> const &) = default;
      
      /*!
       \brief Move constructor
       */
      ts_builder_t(tchecker::graph::ts_builder_t<TS, GRAPH> &&) = default;
      
      /*!
       \brief Destructor
       */
      ~ts_builder_t() = default;
      
      /*!
       \brief Assignment operator
       */
      tchecker::graph::ts_builder_t<TS, GRAPH> & operator= (tchecker::graph::ts_builder_t<TS, GRAPH> const &) = default;
      
      /*!
       \brief Move assignment operator
       */
      tchecker::graph::ts_builder_t<TS, GRAPH> & operator= (tchecker::graph::ts_builder_t<TS, GRAPH> &&) = default;
      
      /*!
       \brief Compute initial node
       \param v : initial iterator value from the transition system
       \param nargs : parameters to GRAPH::allocate_node()
       \param eargs : parameters to GRAPH::allocate_edge()
       \return a triple <node, edge, status> where node and edge are the initial nodes and edges computed from v, and
       status is the tchecker::state_status_t status of node.
       \note node points to nullptr if status != tchecker::STATE_OK
       */
      template <class ... NARGS, class ... EARGS>
      std::tuple<node_ptr_t, edge_ptr_t, tchecker::state_status_t>
      initial_node(typename TS::initial_iterator_value_t const & v, std::tuple<NARGS...> && nargs, std::tuple<EARGS...> && eargs)
      {
        node_ptr_t node = _graph.allocate_node(std::forward<std::tuple<NARGS...>>(nargs));
        edge_ptr_t edge = _graph.allocate_edge(std::forward<std::tuple<EARGS...>>(eargs));
        
        tchecker::state_status_t status = _ts.initialize(*node, *edge, v);
        
        return std::make_tuple((status == tchecker::STATE_OK ? node : node_ptr_t(nullptr)), edge, status);
      }
      
      /*!
       \brief Compute next node
       \param node : source node
       \param v : outgoing edges iterator value from the transition system			 
       \param nargs : parameters to GRAPH::allocate_node()
       \param eargs : parameters to GRAPH::allocate_edge()
       \return a triple <next_node, edge, status> where next_node and edges are the next node and outgoing edge
       of node corresponding to v, and status is the tchecker::state_status_t status of next_node.
       \note next_node points to nullptr if status != tchecker::STATE_OK
       */
      template <class ... NARGS, class ... EARGS>
      std::tuple<node_ptr_t, edge_ptr_t, tchecker::state_status_t>
      next_node(node_ptr_t & node,
                typename TS::outgoing_edges_iterator_value_t const & v,
                std::tuple<NARGS...> && nargs,
                std::tuple<EARGS...> && eargs)
      {
        node_ptr_t next_node = _graph.allocate_from_node(node, std::forward<std::tuple<NARGS...>>(nargs));
        edge_ptr_t edge = _graph.allocate_edge(std::forward<std::tuple<EARGS...>>(eargs));
        
        tchecker::state_status_t status = _ts.next(*next_node, *edge, v);
        
        return std::make_tuple((status == tchecker::STATE_OK ? next_node : node_ptr_t(nullptr)), edge, status);
      }

      /*!
        \brief Get name of state of node.
        \param node: (q,Z)
        \return q from (q,Z)
        Go To zg/details/ts.hh for more details.
      */
      std::string node_name(node_ptr_t & node){
        return _ts.node_name(*node);
      }


      /*!
        \brief Return stack operation of edge
        \param v: outgoing edges iterator value from transition system.
        \return a triplet in the form of pair of pair. The first bool, i.e. first of outer pair,
        describes whether the stack operation is NOP or (Push/Pop). The bool inside inner pair,
        describes whether the stack operation if (Push/Pop) is either Push or Pop, and the string
        which is in the inner pair is the string to be either pushed or popped in case operation is
        (Push/Pop).
        Go To zg/details/ts.hh for more details.
      */
      std::pair<bool, std::pair<bool, std::string>> edge_name(typename TS::outgoing_edges_iterator_value_t const &v){
        return _ts.edge_name(v);
      }


      /*!
        \brief Determine if a node is equal to/subsumes another or not.
        \param n1: (q,Z)
        \param n2: (q',Z')
        \param subsumption: If we need to check for subsumption set this to true, if we need
        to check for equivalence set this value to false.
        \return For equivalence return false if Z~Z', else return true. For subsumption return
        false if Z'<=Z, else return true.
        Go To zg/details/ts.hh for more details.
      */
      bool equal(node_ptr_t n1, node_ptr_t n2, bool subsumption){
        return _ts.equal(*n1, *n2, subsumption);
      }

      /*!
        \brief Determine if two nodes are exactly equal or not.
        \param n1: (q,Z)
        \param n2: (q',Z')
        \return If DBMs of both Z and Z' are exactly the same return true, else return false.
        Go To zg/details/ts.hh for more details.
      */
      bool exact_equal(node_ptr_t n1, node_ptr_t n2){
        return _ts.exact_equal(*n1, *n2);
      }

    protected:
      TS & _ts;         /*!< Transition system */
      GRAPH & _graph;   /*!< Graph with allocation */
    };
    
  } // end of namespace graph
  
} // end of namespace tchecker

#endif // TCHECKER_GRAPH_BUILDER_HH
