//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2018 James D. Mitchell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// TODO:
// 1. Static assert that the class is trivial

#ifndef LIBSEMIGROUPS_INCLUDE_KBE_HPP_
#define LIBSEMIGROUPS_INCLUDE_KBE_HPP_

#include <string>

#include "element.hpp"
#include "knuth-bendix.hpp"
#include "semigroup.hpp"

namespace libsemigroups {
  //! Subclass of Element that wraps an libsemigroups::internal_string_type.
  //!
  //! This class is used to wrap libsemigroups::internal_string_type into an
  //! Element so that it is possible to use them as generators for a Semigroup
  //! object.
  class KBE : public Element {
    using KnuthBendix          = fpsemigroup::KnuthBendix;
    using internal_string_type = KnuthBendix::internal_string_type;

   private:
    KBE(KnuthBendix*, internal_string_type, bool);

   public:
    //! Constructor
    //!
    //! Constructs a KBE which does not represent a word or have an
    //! associated rewriting system.
    KBE();

    //! Constructor from a rewriting system and a word.
    //!
    //! Constructs a KBE which represents the empty word, where multiplication
    //! with other KBE's is defined with respect to the rewriting system \p
    //! kb.
    //!
    //! The rewriting system \p kb is not copied, and it is the
    //! responsibility of the caller to delete it.
    explicit KBE(KnuthBendix*);

    //! Constructor from a rewriting system and a word.
    //!
    //! Constructs a KBE which is essentially the word \p w, whose
    //! multiplication with other KBE's is defined with respect to the
    //! rewriting system \p kb.
    //!
    //! The rewriting system \p kb is not copied, and it is the
    //! responsibility of the caller to delete it.
    KBE(KnuthBendix*, internal_string_type const&);

    //! Constructor from a rewriting system and a letter.
    //!
    //! Calls KBE::KBE with KnuthBendix::uint_to_kb_word of \p a.
    KBE(KnuthBendix*, letter_type const&);

    //! Constructor from a rewriting system and a letter.
    //!
    //! Calls KBE::KBE with KnuthBendix::uint_to_kb_word of \p a.
    KBE(KnuthBendix&, letter_type const&);

    //! Constructor from a rewriting system and a word.
    //!
    //! Calls KBE::KBE with KnuthBendix::word_to_kb_word of \p w.
    KBE(KnuthBendix*, word_type const&);

    //! Constructor from a rewriting system and a word.
    //!
    //! Calls KBE::KBE with KnuthBendix::word_to_kb_word of \p w.
    KBE(KnuthBendix&, word_type const&);

    //! A copy constructor.
    KBE(KBE const&);

    //! Returns \c true if \c this equals \p that.
    //!
    //! This method checks the mathematical equality of two KBE, in other
    //! words whether or not they represent that the same reduced word of the
    //! rewriting system they are defined over.
    bool operator==(Element const& that) const override;

    //! Returns \c true if \c this is less than that and \c false if it is
    //! not.
    //!
    //! This defines a total order on KBEs that is the short-lex order on all
    //! words.
    // TODO should use the reduction ordering of KnuthBendix.
    bool operator<(Element const& that) const override;

    //! Copy \p x into \c this.
    //!
    //! This method copies the KBE pointed to by \p x into \c this by
    //! changing \c this in-place.
    void swap(Element& x) override;

    //! Returns the approximate time complexity of multiplying two
    //! KBE's.
    //!
    //! \sa Element::complexity.
    //!
    //! Returns KBE::LIMIT_MAX since the complexity of multiplying words
    //! in a rewriting system is higher than the cost of tracing a path in the
    //! left or right Cayley graph of a Semigroup.
    size_t complexity() const override;

    //! Returns the degree of an KBE.
    //!
    //! \sa Element::degree.
    //!
    //! Returns the integer 0 since the notion of degree is not really
    //! meaningful in this context.
    size_t degree() const override;

    //! Return the identity KBE.
    //!
    //! \sa Element::identity.
    //!
    //! Returns a new KBE wrapping the empty word and over the same rewriting
    //! system as \c this.
    KBE identity() const;

    //! Returns an independent copy of \c this.
    //!
    //! Returns a pointer to a copy of \c this, which is not linked to \c this
    //! in memory.
    KBE* heap_copy() const override;

    //! Returns a new copy of the identity KBE.
    //!
    //! Returns a pointer to a copy of \c this->identity(), which is not
    //! linked to any other copy in memory.
    KBE* heap_identity() const override;

    //! Calculates a hash value for this object which is cached.
    //!
    //! \sa Element::hash_value and Element::cache_hash_value.
    void cache_hash_value() const override;

    //! Multiply \p x and \p y and stores the result in \c this.
    //!
    //! Redefine \c this to be a reduced word with respect to the rewriting
    //! system of \p x and \p y which is equivalent to the concatenation of
    //! \p x and \p y. This method asserts that \p x and \p y have the same
    //! rewriting system.
    //!
    //! The parameter \p thread_id is required since some temporary storage is
    //! required to find the product of \p x and \p y.  Note that if different
    //! threads call this method with the same value of \p thread_id then bad
    //! things will happen.
    void redefine(Element const& x, Element const& y, size_t tid = 0) override;

#ifdef LIBSEMIGROUPS_DENSEHASHMAP
    Element* empty_key() const override;
#endif

    //! Returns a pointer to the internal_string_type used to create \c this.
    operator word_type() const;
    operator std::string() const;

   private:
    KnuthBendix*         _kb;
    internal_string_type _kb_word;
  };

  template <> struct one<KBE*> {
    KBE* operator()(KBE const* x) {
      return new KBE(std::move(x->identity()));
    }
  };

}  // namespace libsemigroups

namespace std {
  //! Provides a call operator returning a hash value for an Element
  //! via a pointer.
  //!
  //! This struct provides a call operator for obtaining a hash value for the
  //! Element from a const Element pointer. This is used by various methods
  //! of the Semigroup class.
  template <> struct hash<libsemigroups::KBE> {
    //! Hashes a KBE given by const KBE reference.
    size_t operator()(libsemigroups::KBE const& x) const {
      return x.hash_value();
    }
  };

  //! Provides a call operator for comparing KBEs via references.
  //!
  //! This struct provides a call operator for comparing const KBE
  //! references (by comparing the KBE objects they point to). This is used
  //! by various methods of the Semigroup class.
  template <> struct equal_to<libsemigroups::KBE> {
    //! Tests equality of two const KBE references via equality of the KBEs.
    bool operator()(libsemigroups::KBE const& x,
                    libsemigroups::KBE const& y) const {
      return x == y;
    }
  };
}  // namespace std

namespace libsemigroups {
  //! Returns a libsemigroups::word_type which evaluates to \p x.
  //!
  //! Specialises the factorisation method for Semigroup's of KBE's so that it
  //! just returns the word inside the KBE.
  template <> word_type Semigroup<KBE>::factorisation(KBE const& x);
}  // namespace libsemigroups

#endif  // LIBSEMIGROUPS_INCLUDE_KBE_HPP_
