/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ZG_DETAILS_ZG_HH
#define TCHECKER_ZG_DETAILS_ZG_HH

#include <string>
#include <typeinfo>

#include "tchecker/basictypes.hh"
#include "tchecker/utils/iterator.hh"
#include "tchecker/variables/clocks.hh"

/*!
 \file zg.hh
 \brief Zone graph (details)
 */

namespace tchecker {
  
  namespace zg {
    
    namespace details {
      
      /*!
       \class zg_t
       \brief Zone graph (details)
       \tparam TA : type of timed automaton, should inherit from tchecker::ta::details::ta_t
       \tparam ZONE_SEMANTICS : type of zone semantics, should implement tchecker::zone_semantics_t
       */
      template <class TA, class ZONE_SEMANTICS>
      class zg_t {
      public:
        /*!
         \brief Type of model
         */
        using model_t = typename TA::model_t;
        
        /*!
         \brief Type of tuple of locations
         */
        using vloc_t = typename TA::vloc_t;
        
        /*!
         \brief Type of valuation of bounded integer variables
         */
        using intvars_valuation_t = typename TA::intvars_valuation_t;
        
        /*!
         \brief Type of zones
         */
        using zone_t = typename ZONE_SEMANTICS::zone_t;
        
        
        /*!
         \brief Constructor
         \tparam MODEL : type of model, should derive from tchecker::zg::details::model_t
         \param model : a model
         */
        template <class MODEL>
        explicit zg_t(MODEL & model)
        : _ta(model), _zone_semantics(model)
        {}
        
        /*!
         \brief Copy constructor
         */
        zg_t(tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> const &) = default;
        
        /*!
         \brief Move constructor
         */
        zg_t(tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> &&) = default;
        
        /*!
         \brief Destructor
         */
        ~zg_t() = default;
        
        /*!
         \brief Assignment operator
         \param zg : a zone graph
         \post this is a copy of zg
         \return this after assignment
         */
        tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> &
        operator= (tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> const & zg) = default;
        
        /*!
         \brief Move-assignment operator
         \param zg : a zone graph
         \post zg has been moved to this
         \return this after assignment
         */
        tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> &
        operator= (tchecker::zg::details::zg_t<TA, ZONE_SEMANTICS> && zg) = default;
        
        /*!
         \brief Type of iterator over initial states
         */
        using initial_iterator_t = typename TA::initial_iterator_t;
        
        /*!
         \brief Accessor
         \return iterator over initial states
         */
        inline tchecker::range_t<initial_iterator_t> initial() const
        {
          return _ta.initial();
        }
        
        /*!
         \brief Dereference type for iterator over initial states
         */
        using initial_iterator_value_t = typename TA::initial_iterator_value_t;
        
        /*!
         \brief Initialize state
         \param vloc : tuple of locations
         \param intvars_val : valuation of integer variables
         \param zone : a zone
         \param initial_range : range of locations
         \param invariant : a tchecker::clock_constraint_t container
         \pre vloc, intvars_val, zone, and initial_range have dimensions corresponding to number of processes,
         number of integer variables, number of clocks and number of processes in the timed automaton
         \post vloc has been initialized to initial locations in initial_range. intvars_val has been initialized to
         the initial value of integer variables. zone has been initialized to the initial zone according to the chosen
         zone semantics. All constraints in the invariant of vloc have been added to invariant and taken into account for zone
         according to the chosen zone semantics
         \return STATE_OK if vloc, intvars_val and zone have been updated, see tchecker::ta::details::ta_t::initialize
         for other possible values when initializing the integer variables valuation fails, and see
         tchecker::zone_semantics_t::initialize for possible values when initializing the zone fails
         */
        enum tchecker::state_status_t initialize(vloc_t & vloc,
                                                 intvars_valuation_t & intvars_val,
                                                 zone_t & zone,
                                                 initial_iterator_value_t const & initial_range,
                                                 tchecker::clock_constraint_container_t & invariant)
        {
          auto status = _ta.initialize(vloc, intvars_val, initial_range, invariant);
          if (status != tchecker::STATE_OK)
            return status;
          return _zone_semantics.initialize(zone, tchecker::ta::delay_allowed(vloc), invariant, vloc);
        }
        
        /*!
         \brief Type of iterator over outgoing edges
         */
        using outgoing_edges_iterator_t = typename TA::outgoing_edges_iterator_t;
        
        /*!
         \brief Accessor
         \param vloc : tuple of locations
         \return range of outgoing synchronized and asynchronous edges from vloc
         */
        inline tchecker::range_t<outgoing_edges_iterator_t> outgoing_edges(vloc_t const & vloc) const
        {
          return _ta.outgoing_edges(vloc);
        }
        
        /*!
         \brief Dereference type for iterator over outgoing edges
         */
        using outgoing_edges_iterator_value_t = typename TA::outgoing_edges_iterator_value_t;
        
        /*!
         \brief Compute next state
         \param vloc : tuple of locations
         \param intvars_val : valuation of integer variables
         \param zone : a zone
         \param vedge : range of synchronized edges
         \param src_invariant : a tchecker::clock_constraint_t container
         \param guard : a tchecker::clock_constraint_t container
         \param clkreset : a tchecker::clock_reset_t container
         \param tgt_invariant : a tchecker::clock_constraint_t container
         \pre vloc, intvars_val, zone, and vedge have dimensions corresponding to number of processes,
         number of integer variables, number of clocks and number of processes in the timed automaton
         \post vloc, intvars_val and zone have been updated according to the transition that consists in
         the synchronized edges in vedge and according to the chosen zone semantics. All the clock constraints
         from the invariant of vloc before updating have been added to src_invariant. All the clock
         constraints from the invariant of vloc after updating have been added to tgt_invariant.
         All the clcok constraints from the guards in vedge have been added to guard. All the
         clock resets from the resets in vedge have been added to clkreset.
         \return STATE_OK if vloc, intvars_val and zone have been updated, see tchecker::ta::details::ta_t::next
         for other possible values when updating the integer variables valuation fails, and see
         tchecker::zone_semantics_t::next for possible values when updating the zone fails
         \note this method does not clear src_invariant, guard, clkreset and tgt_invariant. It adds clock
         contraints/resets to these containers. Every clock constraint/reset in the containers when the
         function is called are taken into account to update zone
         */
        enum tchecker::state_status_t next(vloc_t & vloc,
                                           intvars_valuation_t & intvars_val,
                                           zone_t & zone,
                                           outgoing_edges_iterator_value_t const & vedge,
                                           tchecker::clock_constraint_container_t & src_invariant,
                                           tchecker::clock_constraint_container_t & guard,
                                           tchecker::clock_reset_container_t & clkreset,
                                           tchecker::clock_constraint_container_t & tgt_invariant)
        {
            // std::cout << "EDGE TYPE:\t" << typeid(((vedge.begin())._edge)->_symbol).name() << "\n";
            // if(((vedge.begin())._edge)->_stop)    std::cout << "EDGE SYMBOL:\t" << ((vedge.begin())._edge)->_symbol << "\n";
          bool src_delay_allowed = tchecker::ta::delay_allowed(vloc);
          auto status = _ta.next(vloc, intvars_val, vedge, src_invariant, guard, clkreset, tgt_invariant);
          if (status != tchecker::STATE_OK)
            return status;
          bool tgt_delay_allowed = tchecker::ta::delay_allowed(vloc);
          return _zone_semantics.next(zone, src_delay_allowed, src_invariant, guard, clkreset,
                                      tgt_delay_allowed, tgt_invariant, vloc);
        }

        /*!
          \brief Return stack operation of edge
          \param vedge: Range of edges.
          \return a triplet in the form of pair of pair. The first bool, i.e. first of outer pair,
          describes whether the stack operation is NOP or (Push/Pop). The bool inside inner pair,
          describes whether the stack operation if (Push/Pop) is either Push or Pop, and the string
          which is in the inner pair is the string to be either pushed or popped in case operation is
          (Push/Pop).
        */
        std::pair<bool, std::pair<bool, std::string>> edge_name(outgoing_edges_iterator_value_t const & vedge){
            std::string symbol = "";
            bool b = (vedge.begin()._edge)->_stop;
            bool porp = 0;
            if(b){
                symbol = (vedge.begin()._edge)->_symbol;
                porp = (vedge.begin()._edge)->_push_or_pop;
            }
            return std::make_pair(b, make_pair(porp, symbol));
        }

        /*!
          \brief Determine if a node is equal to/subsumes another or not.
          \param z1: (q,Z)
          \param z2: (q',Z')
          \param vloc1: tuple of locations for z1
          \param vloc2: tuple of locations for z2
          \param subsumption: If we need to check for subsumption set this to true, if we need
          to check for equivalence set this value to false.
          \return For equivalence return false if Z~Z', else return true. For subsumption return
          false if Z'<=Z, else return true.
          Go To zone/dbm/semantics.hh for more details.
        */
        bool equal(zone_t & z1, zone_t & z2, vloc_t & vloc1, vloc_t & vloc2, bool subsumption){
            return _zone_semantics.equal(z1, z2, subsumption);
        }

        /*!
          \brief Determine if two nodes are exactly equal or not.
          \param z1: (q,Z)
          \param z2: (q',Z')
          \return If DBMs of both Z and Z' are exactly the same return true, else return false.
          Go To zone/dbm/semantics.hh for more details.
        */
        bool exact_equal(zone_t & z1, zone_t & z2){
            return _zone_semantics.exact_equal(z1, z2);
        }

        /*!
          \brief Get name of state of node.
          \param vloc1: tuple of locations for (q,Z)
          \return q from (q,Z)
        */
        std::string node_name(vloc_t &vloc1){
            auto begin1 = vloc1.begin(), end1 = vloc1.end();
            std::string s1 = "";
            for(auto it = begin1 ; it != end1 ; it++){
                if(it != begin1)    s1 += ",";
                s1 += (*it)->name();
            }
            return s1;
        }
        
        /*!
         \brief Accessor
         \return Underlying model
         */
        inline constexpr model_t const & model() const
        {
          return _ta.model();
        }
      protected:
        TA _ta;                          /*!< Timed automaton */
        ZONE_SEMANTICS _zone_semantics;  /*!< Zone semantics */
      };
      
    } // end of namespace details
    
  } // end of namespace zg
  
} // end of namespace tchecker

#endif // TCHECKER_ZG_DETAILS_ZG_HH
