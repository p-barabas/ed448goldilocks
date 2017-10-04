/**
 * @file decaf/point_255.hxx
 * @author Mike Hamburg
 *
 * @copyright
 *   Copyright (c) 2015-2016 Cryptography Research, Inc.  \n
 *   Released under the MIT License.  See LICENSE.txt for license information.
 *
 * A group of prime order p, C++ wrapper.
 *
 * The Decaf library implements cryptographic operations on a an elliptic curve
 * group of prime order p. It accomplishes this by using a twisted Edwards
 * curve (isogenous to Curve25519) and wiping out the cofactor.
 *
 * The formulas are all complete and have no special cases, except that
 * decaf_255_decode can fail because not every sequence of bytes is a valid group
 * element.
 *
 * The formulas contain no data-dependent branches, timing or memory accesses,
 * except for decaf_255_base_double_scalarmul_non_secret.
 *
 * @warning This file was automatically generated in Python.
 * Please do not edit it.
 */

#ifndef __DECAF_POINT_255_HXX__
#define __DECAF_POINT_255_HXX__ 1

/** This code uses posix_memalign. */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
#include <stdlib.h>
#include <string.h> /* for memcpy */

#include <decaf/point_255.h>
#include <decaf/ed255.h>
#include <decaf/secure_buffer.hxx>
#include <string>
#include <sys/types.h>
#include <limits.h>

/** @cond internal */
#if __cplusplus >= 201103L
#define DECAF_NOEXCEPT noexcept
#else
#define DECAF_NOEXCEPT throw()
#endif
/** @endcond */

namespace decaf {

/**
 * Curve25519/Decaf instantiation of group.
 */
struct IsoEd25519 {

/** The name of the curve */
static inline const char *name() { return "Iso-Ed25519"; }

/** The name of the curve */
static inline int bits() { return 255; }

/** The curve's cofactor (removed, but useful for testing) */
static const int REMOVED_COFACTOR = 8;

/** Residue class of field modulus: p == this mod 2*(this-1) */
static const int FIELD_MODULUS_TYPE = 5;

/** @cond internal */
class Point;
class Precomputed;
/** @endcond */

/**
 * A scalar modulo the curve order.
 * Supports the usual arithmetic operations, all in constant time.
 */
class Scalar : public Serializable<Scalar> {
public:
    /** wrapped C type */
    typedef decaf_255_scalar_t Wrapped;
    
    /** Size of a serialized element */
    static const size_t SER_BYTES = DECAF_255_SCALAR_BYTES;

    /** access to the underlying scalar object */
    Wrapped s;

    /** @cond internal */
    /** Don't initialize. */
    inline Scalar(const NOINIT &) DECAF_NOEXCEPT {}
    /** @endcond */

    /** Set to an unsigned word */
    inline Scalar(uint64_t w) DECAF_NOEXCEPT { *this = w; }

    /** Set to a signed word */
    inline Scalar(int64_t w) DECAF_NOEXCEPT { *this = w; }

    /** Set to an unsigned word */
    inline Scalar(unsigned int w) DECAF_NOEXCEPT { *this = w; }

    /** Set to a signed word */
    inline Scalar(int w) DECAF_NOEXCEPT { *this = w; }

    /** Construct from RNG */
    inline explicit Scalar(Rng &rng) DECAF_NOEXCEPT {
        FixedArrayBuffer<SER_BYTES + 16> sb(rng);
        *this = sb;
    }

    /** Construct from decaf_scalar_t object. */
    inline Scalar(const Wrapped &t = decaf_255_scalar_zero) DECAF_NOEXCEPT { decaf_255_scalar_copy(s,t); }

    /** Copy constructor. */
    inline Scalar(const Scalar &x) DECAF_NOEXCEPT { *this = x; }

    /** Construct from arbitrary-length little-endian byte sequence. */
    inline Scalar(const Block &buffer) DECAF_NOEXCEPT { *this = buffer; }

    /** Serializable instance */
    inline size_t ser_size() const DECAF_NOEXCEPT { return SER_BYTES; }

    /** Serializable instance */
    inline void serialize_into(unsigned char *buffer) const DECAF_NOEXCEPT {
        decaf_255_scalar_encode(buffer, s);
    }

    /** Assignment. */
    inline Scalar& operator=(const Scalar &x) DECAF_NOEXCEPT { decaf_255_scalar_copy(s,x.s); return *this; }

    /** Assign from unsigned 64-bit integer. */
    inline Scalar& operator=(uint64_t w) DECAF_NOEXCEPT { decaf_255_scalar_set_unsigned(s,w); return *this; }


    /** Assign from signed int. */
    inline Scalar& operator=(int64_t w) DECAF_NOEXCEPT {
        Scalar t(-(uint64_t)INT_MIN);
        decaf_255_scalar_set_unsigned(s,(uint64_t)w - (uint64_t)INT_MIN);
        *this -= t;
        return *this;
    }

    /** Assign from unsigned int. */
    inline Scalar& operator=(unsigned int w) DECAF_NOEXCEPT { return *this = (uint64_t)w; }

    /** Assign from signed int. */
    inline Scalar& operator=(int w) DECAF_NOEXCEPT { return *this = (int64_t)w; }

    /** Destructor securely zeorizes the scalar. */
    inline ~Scalar() DECAF_NOEXCEPT { decaf_255_scalar_destroy(s); }

    /** Assign from arbitrary-length little-endian byte sequence in a Block. */
    inline Scalar &operator=(const Block &bl) DECAF_NOEXCEPT {
        decaf_255_scalar_decode_long(s,bl.data(),bl.size()); return *this;
    }

    /**
     * Decode from correct-length little-endian byte sequence.
     * @return DECAF_FAILURE if the scalar is greater than or equal to the group order q.
     */
    static inline decaf_error_t DECAF_WARN_UNUSED decode (
        Scalar &sc, const FixedBlock<SER_BYTES> buffer
    ) DECAF_NOEXCEPT {
        return decaf_255_scalar_decode(sc.s,buffer.data());
    }

    /** Add. */
    inline Scalar operator+ (const Scalar &q) const DECAF_NOEXCEPT { Scalar r((NOINIT())); decaf_255_scalar_add(r.s,s,q.s); return r; }

    /** Add to this. */
    inline Scalar &operator+=(const Scalar &q) DECAF_NOEXCEPT { decaf_255_scalar_add(s,s,q.s); return *this; }

    /** Subtract. */
    inline Scalar operator- (const Scalar &q) const DECAF_NOEXCEPT { Scalar r((NOINIT())); decaf_255_scalar_sub(r.s,s,q.s); return r; }

    /** Subtract from this. */
    inline Scalar &operator-=(const Scalar &q) DECAF_NOEXCEPT { decaf_255_scalar_sub(s,s,q.s); return *this; }

    /** Multiply */
    inline Scalar operator* (const Scalar &q) const DECAF_NOEXCEPT { Scalar r((NOINIT())); decaf_255_scalar_mul(r.s,s,q.s); return r; }

    /** Multiply into this. */
    inline Scalar &operator*=(const Scalar &q) DECAF_NOEXCEPT { decaf_255_scalar_mul(s,s,q.s); return *this; }

    /** Negate */
    inline Scalar operator- () const DECAF_NOEXCEPT { Scalar r((NOINIT())); decaf_255_scalar_sub(r.s,decaf_255_scalar_zero,s); return r; }

    /** Invert with Fermat's Little Theorem (slow!). If *this == 0,
     * throw CryptoException. */
    inline Scalar inverse() const /*throw(CryptoException)*/ {
        Scalar r;
        if (DECAF_SUCCESS != decaf_255_scalar_invert(r.s,s)) {
            throw CryptoException();
        }
        return r;
    }

    /** Invert with Fermat's Little Theorem (slow!). If *this == 0, set r=0
     * and return DECAF_FAILURE. */
    inline decaf_error_t DECAF_WARN_UNUSED
    inverse_noexcept(Scalar &r) const DECAF_NOEXCEPT {
        return decaf_255_scalar_invert(r.s,s);
    }

    /** Divide by inverting q. If q == 0, return 0. */
    inline Scalar operator/ (const Scalar &q) const /*throw(CryptoException)*/ { return *this * q.inverse(); }

    /** Divide by inverting q. If q == 0, return 0. */
    inline Scalar &operator/=(const Scalar &q) /*throw(CryptoException)*/ { return *this *= q.inverse(); }

    /** Return half this scalar.  Much faster than /2. */
    inline Scalar half() const { Scalar out; decaf_255_scalar_halve(out.s,s); return out; }

    /** Compare in constant time */
    inline bool operator!=(const Scalar &q) const DECAF_NOEXCEPT { return !(*this == q); }

    /** Compare in constant time */
    inline bool operator==(const Scalar &q) const DECAF_NOEXCEPT { return !!decaf_255_scalar_eq(s,q.s); }

    /** Scalarmul with scalar on left. */
    inline Point operator* (const Point &q) const DECAF_NOEXCEPT { return q * (*this); }

    /** Scalarmul-precomputed with scalar on left. */
    inline Point operator* (const Precomputed &q) const DECAF_NOEXCEPT { return q * (*this); }

    /** Direct scalar multiplication. */
    inline SecureBuffer direct_scalarmul (
        const FixedBlock<SER_BYTES> &in,
        decaf_bool_t allow_identity=DECAF_FALSE,
        decaf_bool_t short_circuit=DECAF_TRUE
    ) const /*throw(CryptoException)*/;
        
    /** Direct scalar multiplication. */
    inline decaf_error_t DECAF_WARN_UNUSED direct_scalarmul_noexcept(
        FixedBuffer<SER_BYTES> &out,
        const FixedBlock<SER_BYTES> &in,
        decaf_bool_t allow_identity=DECAF_FALSE,
        decaf_bool_t short_circuit=DECAF_TRUE
    ) const DECAF_NOEXCEPT;
};

/**
 * Element of prime-order group.
 */
class Point : public Serializable<Point> {
public:
    /** wrapped C type */
    typedef decaf_255_point_t Wrapped;
    
    /** Size of a serialized element */
    static const size_t SER_BYTES = DECAF_255_SER_BYTES;

    /** Bytes required for hash */
    static const size_t HASH_BYTES = DECAF_255_HASH_BYTES;

    /**
     * Size of a stegged element.
     * 
     * FUTURE: You can use HASH_BYTES * 3/2 (or more likely much less, eg HASH_BYTES + 8)
     * with a random oracle hash function, by hash-expanding everything past the first
     * HASH_BYTES of the element.  However, since the internal C invert_elligator is not
     * tied to a hash function, I didn't want to tie the C++ wrapper to a hash function
     * either.  But it might be a good idea to do this in the future, either with STROBE
     * or something else.
     *
     * Then again, calling invert_elligator at all is super niche, so maybe who cares?
     */
    static const size_t STEG_BYTES = HASH_BYTES * 2;
    
    /** Number of bits in invert_elligator which are actually used. */
    static const unsigned int INVERT_ELLIGATOR_WHICH_BITS = DECAF_255_INVERT_ELLIGATOR_WHICH_BITS;

    /** The c-level object. */
    Wrapped p;

    /** @cond internal */
    /** Don't initialize. */
    inline Point(const NOINIT &) DECAF_NOEXCEPT {}
    /** @endcond */

    /** Constructor sets to identity by default. */
    inline Point(const Wrapped &q = decaf_255_point_identity) DECAF_NOEXCEPT { decaf_255_point_copy(p,q); }

    /** Copy constructor. */
    inline Point(const Point &q) DECAF_NOEXCEPT { *this = q; }

    /** Assignment. */
    inline Point& operator=(const Point &q) DECAF_NOEXCEPT { decaf_255_point_copy(p,q.p); return *this; }

    /** Destructor securely zeorizes the point. */
    inline ~Point() DECAF_NOEXCEPT { decaf_255_point_destroy(p); }

    /** Construct from RNG */
    inline explicit Point(Rng &rng, bool uniform = true) DECAF_NOEXCEPT {
        if (uniform) {
            FixedArrayBuffer<2*HASH_BYTES> b(rng);
            set_to_hash(b);
        } else {
            FixedArrayBuffer<HASH_BYTES> b(rng);
            set_to_hash(b);
        }
    }

   /**
    * Initialize from a fixed-length byte string.
    * The all-zero string maps to the identity.
    *
    * @throw CryptoException the string was the wrong length, or wasn't the encoding of a point,
    * or was the identity and allow_identity was DECAF_FALSE.
    */
    inline explicit Point(const FixedBlock<SER_BYTES> &buffer, decaf_bool_t allow_identity=DECAF_TRUE)
        /*throw(CryptoException)*/ {
        if (DECAF_SUCCESS != decode(buffer,allow_identity)) {
            throw CryptoException();
        }
    }

    /**
     * Initialize from C++ fixed-length byte string.
     * The all-zero string maps to the identity.
     *
     * @retval DECAF_SUCCESS the string was successfully decoded.
     * @return DECAF_FAILURE the string was the wrong length, or wasn't the encoding of a point,
     * or was the identity and allow_identity was DECAF_FALSE. Contents of the buffer are undefined.
     */
    inline decaf_error_t DECAF_WARN_UNUSED decode (
        const FixedBlock<SER_BYTES> &buffer, decaf_bool_t allow_identity=DECAF_TRUE
    ) DECAF_NOEXCEPT {
        return decaf_255_point_decode(p,buffer.data(),allow_identity);
    }

    /**
     * Initialize from C++ fixed-length byte string, like EdDSA.
     * The all-zero string maps to the identity.
     *
     * @retval DECAF_SUCCESS the string was successfully decoded.
     * @return DECAF_FAILURE the string was the wrong length, or wasn't the encoding of a point.
     * Contents of the point are undefined.
     */
    inline decaf_error_t DECAF_WARN_UNUSED decode_like_eddsa_and_ignore_cofactor_noexcept (
        const FixedBlock<DECAF_EDDSA_25519_PUBLIC_BYTES> &buffer
    ) DECAF_NOEXCEPT {
        return decaf_255_point_decode_like_eddsa_and_ignore_cofactor(p,buffer.data());
    }

    inline void decode_like_eddsa_and_ignore_cofactor (
        const FixedBlock<DECAF_EDDSA_25519_PUBLIC_BYTES> &buffer
    ) /*throw(CryptoException)*/ {
        if (DECAF_SUCCESS != decode_like_eddsa_and_ignore_cofactor_noexcept(buffer)) throw(CryptoException());
    }

    /** Multiply out cofactor and encode like EdDSA. */
    inline SecureBuffer mul_by_cofactor_and_encode_like_eddsa() const {
        SecureBuffer ret(DECAF_EDDSA_25519_PUBLIC_BYTES);
        decaf_255_point_mul_by_cofactor_and_encode_like_eddsa(ret.data(),p);
        return ret;
    }

    /**
     * Map uniformly to the curve from a hash buffer.
     * The empty or all-zero string maps to the identity, as does the string "\\x01".
     * If the buffer is shorter than 2*HASH_BYTES, well, it won't be as uniform,
     * but the buffer will be zero-padded on the right.
     */
    static inline Point from_hash ( const Block &s ) DECAF_NOEXCEPT {
        Point p((NOINIT())); p.set_to_hash(s); return p;
    }

    /**
     * Map to the curve from a hash buffer.
     * The empty or all-zero string maps to the identity, as does the string "\\x01".
     * If the buffer is shorter than 2*HASH_BYTES, well, it won't be as uniform,
     * but the buffer will be zero-padded on the right.
     */
    inline void set_to_hash( const Block &s ) DECAF_NOEXCEPT {
        if (s.size() < HASH_BYTES) {
            SecureBuffer b(HASH_BYTES);
            memcpy(b.data(), s.data(), s.size());
            decaf_255_point_from_hash_nonuniform(p,b.data());
        } else if (s.size() == HASH_BYTES) {
            decaf_255_point_from_hash_nonuniform(p,s.data());
        } else if (s.size() < 2*HASH_BYTES) {
            SecureBuffer b(2*HASH_BYTES);
            memcpy(b.data(), s.data(), s.size());
            decaf_255_point_from_hash_uniform(p,b.data());
        } else {
            decaf_255_point_from_hash_uniform(p,s.data());
        }
    }

    /**
     * Encode to string. The identity encodes to the all-zero string.
     */
    inline operator SecureBuffer() const {
        SecureBuffer buffer(SER_BYTES);
        decaf_255_point_encode(buffer.data(), p);
        return buffer;
    }

    /** Serializable instance */
    inline size_t ser_size() const DECAF_NOEXCEPT { return SER_BYTES; }

    /** Serializable instance */
    inline void serialize_into(unsigned char *buffer) const DECAF_NOEXCEPT {
        decaf_255_point_encode(buffer, p);
    }

    /** Point add. */
    inline Point operator+ (const Point &q) const DECAF_NOEXCEPT { Point r((NOINIT())); decaf_255_point_add(r.p,p,q.p); return r; }

    /** Point add. */
    inline Point &operator+=(const Point &q) DECAF_NOEXCEPT { decaf_255_point_add(p,p,q.p); return *this; }

    /** Point subtract. */
    inline Point operator- (const Point &q) const DECAF_NOEXCEPT { Point r((NOINIT())); decaf_255_point_sub(r.p,p,q.p); return r; }

    /** Point subtract. */
    inline Point &operator-=(const Point &q) DECAF_NOEXCEPT { decaf_255_point_sub(p,p,q.p); return *this; }

    /** Point negate. */
    inline Point operator- () const DECAF_NOEXCEPT { Point r((NOINIT())); decaf_255_point_negate(r.p,p); return r; }

    /** Double the point out of place. */
    inline Point times_two () const DECAF_NOEXCEPT { Point r((NOINIT())); decaf_255_point_double(r.p,p); return r; }

    /** Double the point in place. */
    inline Point &double_in_place() DECAF_NOEXCEPT { decaf_255_point_double(p,p); return *this; }

    /** Constant-time compare. */
    inline bool operator!=(const Point &q) const DECAF_NOEXCEPT { return ! decaf_255_point_eq(p,q.p); }

    /** Constant-time compare. */
    inline bool operator==(const Point &q) const DECAF_NOEXCEPT { return !!decaf_255_point_eq(p,q.p); }

    /** Scalar multiply. */
    inline Point operator* (const Scalar &s) const DECAF_NOEXCEPT { Point r((NOINIT())); decaf_255_point_scalarmul(r.p,p,s.s); return r; }

    /** Scalar multiply in place. */
    inline Point &operator*=(const Scalar &s) DECAF_NOEXCEPT { decaf_255_point_scalarmul(p,p,s.s); return *this; }

    /** Multiply by s.inverse(). If s=0, maps to the identity. */
    inline Point operator/ (const Scalar &s) const /*throw(CryptoException)*/ { return (*this) * s.inverse(); }

    /** Multiply by s.inverse(). If s=0, maps to the identity. */
    inline Point &operator/=(const Scalar &s) /*throw(CryptoException)*/ { return (*this) *= s.inverse(); }

    /** Validate / sanity check */
    inline bool validate() const DECAF_NOEXCEPT { return decaf_255_point_valid(p); }

    /** Double-scalar multiply, equivalent to q*qs + r*rs but faster. */
    static inline Point double_scalarmul (
        const Point &q, const Scalar &qs, const Point &r, const Scalar &rs
    ) DECAF_NOEXCEPT {
        Point p((NOINIT())); decaf_255_point_double_scalarmul(p.p,q.p,qs.s,r.p,rs.s); return p;
    }

    /** Dual-scalar multiply, equivalent to this*r1, this*r2 but faster. */
    inline void dual_scalarmul (
        Point &q1, Point &q2, const Scalar &r1, const Scalar &r2
    ) const DECAF_NOEXCEPT {
        decaf_255_point_dual_scalarmul(q1.p,q2.p,p,r1.s,r2.s);
    }

    /**
     * Double-scalar multiply, equivalent to q*qs + r*rs but faster.
     * For those who like their scalars before the point.
     */
    static inline Point double_scalarmul (
        const Scalar &qs, const Point &q, const Scalar &rs, const Point &r
    ) DECAF_NOEXCEPT {
        return double_scalarmul(q,qs,r,rs);
    }

    /**
     * Double-scalar multiply: this point by the first scalar and base by the second scalar.
     * @warning This function takes variable time, and may leak the scalars (or points, but currently
     * it doesn't).
     */
    inline Point non_secret_combo_with_base(const Scalar &s, const Scalar &s_base) DECAF_NOEXCEPT {
        Point r((NOINIT())); decaf_255_base_double_scalarmul_non_secret(r.p,s_base.s,p,s.s); return r;
    }

    /** Return a point equal to *this, whose internal data is rotated by a torsion element. */
    inline Point debugging_torque() const DECAF_NOEXCEPT {
        Point q;
        decaf_255_point_debugging_torque(q.p,p);
        return q;
    }

    /** Return a point equal to *this, whose internal data has a modified representation. */
    inline Point debugging_pscale(const FixedBlock<SER_BYTES> factor) const DECAF_NOEXCEPT {
        Point q;
        decaf_255_point_debugging_pscale(q.p,p,factor.data());
        return q;
    }

    /** Return a point equal to *this, whose internal data has a randomized representation. */
    inline Point debugging_pscale(Rng &r) const DECAF_NOEXCEPT {
        FixedArrayBuffer<SER_BYTES> sb(r);
        return debugging_pscale(sb);
    }

    /**
     * Modify buffer so that Point::from_hash(Buffer) == *this, and return DECAF_SUCCESS;
     * or leave buf unmodified and return DECAF_FAILURE.
     */
    inline decaf_error_t invert_elligator (
        Buffer buf, uint32_t hint
    ) const DECAF_NOEXCEPT {
        unsigned char buf2[2*HASH_BYTES];
        memset(buf2,0,sizeof(buf2));
        memcpy(buf2,buf.data(),(buf.size() > 2*HASH_BYTES) ? 2*HASH_BYTES : buf.size());
        decaf_bool_t ret;
        if (buf.size() > HASH_BYTES) {
            ret = decaf_successful(decaf_255_invert_elligator_uniform(buf2, p, hint));
        } else {
            ret = decaf_successful(decaf_255_invert_elligator_nonuniform(buf2, p, hint));
        }
        if (buf.size() < HASH_BYTES) {
            ret &= decaf_memeq(&buf2[buf.size()], &buf2[HASH_BYTES], HASH_BYTES - buf.size());
        }
        for (size_t i=0; i<buf.size() && i<HASH_BYTES; i++) {
            buf[i] = (buf[i] & ~ret) | (buf2[i] &ret);
        }
        decaf_bzero(buf2,sizeof(buf2));
        return decaf_succeed_if(ret);
    }

    /** Steganographically encode this */
    inline SecureBuffer steg_encode(Rng &rng, size_t size=STEG_BYTES) const /*throw(std::bad_alloc, LengthException)*/ {
        if (size <= HASH_BYTES + 4 || size > 2*HASH_BYTES) throw LengthException();
        SecureBuffer out(STEG_BYTES);
        decaf_error_t done;
        do {
            rng.read(Buffer(out).slice(HASH_BYTES-4,STEG_BYTES-HASH_BYTES+1));
            uint32_t hint = 0;
            for (int i=0; i<4; i++) { hint |= uint32_t(out[HASH_BYTES-4+i])<<(8*i); }
            done = invert_elligator(out, hint);
        } while (!decaf_successful(done));
        return out;
    }

    /** Return the base point */
    static inline const Point base() DECAF_NOEXCEPT { return Point(decaf_255_point_base); }

    /** Return the identity point */
    static inline const Point identity() DECAF_NOEXCEPT { return Point(decaf_255_point_identity); }
};

/**
 * Precomputed table of points.
 * Minor difficulties arise here because the decaf API doesn't expose, as a constant, how big such an object is.
 * Therefore we have to call malloc() or friends, but that's probably for the best, because you don't want to
 * stack-allocate a 15kiB object anyway.
 */

/** @cond internal */
typedef decaf_255_precomputed_s Precomputed_U;
/** @endcond */
class Precomputed
    /** @cond internal */
    : protected OwnedOrUnowned<Precomputed,Precomputed_U>
    /** @endcond */
{
public:

    /** Destructor securely zeorizes the memory. */
    inline ~Precomputed() DECAF_NOEXCEPT { clear(); }

    /**
     * Initialize from underlying type, declared as a reference to prevent
     * it from being called with 0, thereby breaking override.
     *
     * The underlying object must remain valid throughout the lifetime of this one.
     *
     * By default, initializes to the table for the base point.
     *
     * @warning The empty initializer makes this equal to base, unlike the empty
     * initializer for points which makes this equal to the identity.
     */
    inline Precomputed (
        const Precomputed_U &yours = *decaf_255_precomputed_base
    ) DECAF_NOEXCEPT : OwnedOrUnowned<Precomputed,Precomputed_U>(yours) {}


#if __cplusplus >= 201103L
    /** Move-assign operator */
    inline Precomputed &operator=(Precomputed &&it) DECAF_NOEXCEPT {
        OwnedOrUnowned<Precomputed,Precomputed_U>::operator= (it);
        return *this;
    }

    /** Move constructor */
    inline Precomputed(Precomputed &&it) DECAF_NOEXCEPT : OwnedOrUnowned<Precomputed,Precomputed_U>() {
        *this = it;
    }

    /** Undelete copy operator */
    inline Precomputed &operator=(const Precomputed &it) DECAF_NOEXCEPT {
        OwnedOrUnowned<Precomputed,Precomputed_U>::operator= (it);
        return *this;
    }
#endif

    /**
     * Initilaize from point. Must allocate memory, and may throw.
     */
    inline Precomputed &operator=(const Point &it) /*throw(std::bad_alloc)*/ {
        alloc();
        decaf_255_precompute(ours.mine,it.p);
        return *this;
    }

    /**
     * Copy constructor.
     */
    inline Precomputed(const Precomputed &it) /*throw(std::bad_alloc)*/
        : OwnedOrUnowned<Precomputed,Precomputed_U>() { *this = it; }

    /**
     * Constructor which initializes from point.
     */
    inline explicit Precomputed(const Point &it) /*throw(std::bad_alloc)*/
        : OwnedOrUnowned<Precomputed,Precomputed_U>() { *this = it; }

    /** Fixed base scalarmul. */
    inline Point operator* (const Scalar &s) const DECAF_NOEXCEPT { Point r; decaf_255_precomputed_scalarmul(r.p,get(),s.s); return r; }

    /** Multiply by s.inverse(). If s=0, maps to the identity. */
    inline Point operator/ (const Scalar &s) const /*throw(CryptoException)*/ { return (*this) * s.inverse(); }

    /** Return the table for the base point. */
    static inline const Precomputed base() DECAF_NOEXCEPT { return Precomputed(); }

public:
    /** @cond internal */
    friend class OwnedOrUnowned<Precomputed,Precomputed_U>;
    static inline size_t size() DECAF_NOEXCEPT { return decaf_255_sizeof_precomputed_s; }
    static inline size_t alignment() DECAF_NOEXCEPT { return decaf_255_alignof_precomputed_s; }
    static inline const Precomputed_U * default_value() DECAF_NOEXCEPT { return decaf_255_precomputed_base; }
    /** @endcond */
};

struct DhLadder {
public:
    /** Bytes in an X25519 public key. */
    static const size_t PUBLIC_BYTES = DECAF_X25519_PUBLIC_BYTES;

    /** Bytes in an X25519 private key. */
    static const size_t PRIVATE_BYTES = DECAF_X25519_PRIVATE_BYTES;

    /** Base point for a scalar multiplication. */
    static const FixedBlock<PUBLIC_BYTES> base_point() DECAF_NOEXCEPT {
        return FixedBlock<PUBLIC_BYTES>(decaf_x25519_base_point);
    }

    /** Calculate and return a shared secret with public key.  */
    static inline SecureBuffer shared_secret(
        const FixedBlock<PUBLIC_BYTES> &pk,
        const FixedBlock<PRIVATE_BYTES> &scalar
    ) /*throw(std::bad_alloc,CryptoException)*/ {
        SecureBuffer out(PUBLIC_BYTES);
        if (DECAF_SUCCESS != decaf_x25519(out.data(), pk.data(), scalar.data())) {
            throw CryptoException();
        }
        return out;
    }

    /** Calculate and write into out a shared secret with public key, noexcept version.  */
    static inline decaf_error_t DECAF_WARN_UNUSED
    shared_secret_noexcept (
        FixedBuffer<PUBLIC_BYTES> &out,
        const FixedBlock<PUBLIC_BYTES> &pk,
        const FixedBlock<PRIVATE_BYTES> &scalar
    ) DECAF_NOEXCEPT {
       return decaf_x25519(out.data(), pk.data(), scalar.data());
    }

    /** Calculate and return a public key; equivalent to shared_secret(base_point(),scalar)
     * but possibly faster.
     * @deprecated Renamed to derive_public_key.
     */
    static inline SecureBuffer DECAF_DEPRECATED("Renamed to derive_public_key")
    generate_key(
        const FixedBlock<PRIVATE_BYTES> &scalar
    ) /*throw(std::bad_alloc)*/ {
        SecureBuffer out(PUBLIC_BYTES);
        decaf_x25519_derive_public_key(out.data(), scalar.data());
        return out;
    }

    /** Calculate and return a public key; equivalent to shared_secret(base_point(),scalar)
     * but possibly faster.
     */
    static inline SecureBuffer derive_public_key(
        const FixedBlock<PRIVATE_BYTES> &scalar
    ) /*throw(std::bad_alloc)*/ {
        SecureBuffer out(PUBLIC_BYTES);
        decaf_x25519_derive_public_key(out.data(), scalar.data());
        return out;
    }

    /** Calculate and return a public key into a fixed buffer;
     * equivalent to shared_secret(base_point(),scalar) but possibly faster.
     */
    static inline void
    derive_public_key_noexcept (
        FixedBuffer<PUBLIC_BYTES> &out,
        const FixedBlock<PRIVATE_BYTES> &scalar
    ) DECAF_NOEXCEPT {
        decaf_x25519_derive_public_key(out.data(), scalar.data());
    }

    /** Calculate and return a public key into a fixed buffer;
     * equivalent to shared_secret(base_point(),scalar) but possibly faster.
     * @deprecated Renamed to derive_public_key_noexcept.
     */
    static inline void DECAF_DEPRECATED("Renamed to derive_public_key_noexcept")
    generate_key_noexcept (
        FixedBuffer<PUBLIC_BYTES> &out,
        const FixedBlock<PRIVATE_BYTES> &scalar
    ) DECAF_NOEXCEPT {
        decaf_x25519_derive_public_key(out.data(), scalar.data());
    }
};

}; /* struct IsoEd25519 */

/** @cond internal */
inline SecureBuffer IsoEd25519::Scalar::direct_scalarmul (
    const FixedBlock<IsoEd25519::Point::SER_BYTES> &in,
    decaf_bool_t allow_identity,
    decaf_bool_t short_circuit
) const /*throw(CryptoException)*/ {
    SecureBuffer out(IsoEd25519::Point::SER_BYTES);
    if (DECAF_SUCCESS !=
        decaf_255_direct_scalarmul(out.data(), in.data(), s, allow_identity, short_circuit)
    ) {
        throw CryptoException();
    }
    return out;
}

inline decaf_error_t IsoEd25519::Scalar::direct_scalarmul_noexcept (
    FixedBuffer<IsoEd25519::Point::SER_BYTES> &out,
    const FixedBlock<IsoEd25519::Point::SER_BYTES> &in,
    decaf_bool_t allow_identity,
    decaf_bool_t short_circuit
) const DECAF_NOEXCEPT {
    return decaf_255_direct_scalarmul(out.data(), in.data(), s, allow_identity, short_circuit);
}
/** @endcond */

#undef DECAF_NOEXCEPT
} /* namespace decaf */

#endif /* __DECAF_POINT_255_HXX__ */
