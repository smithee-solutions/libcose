/*
 * Copyright (C) 2018 Freie Universitat Berlin
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    cose_key COSE key defintions
 * @ingroup     cose
 * Internal constants for signing
 * @{
 *
 * @file
 * @brief       API definitions for COSE key objects
 *
 * @author      Koen Zandberg <koen@bergzand.net>
*/

#ifndef COSE_SIGNER_H
#define COSE_SIGNER_H

#include "cose_defines.h"
#include <nanocbor/nanocbor.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name COSE key object
 *
 * For signing, only the d parameter is required and the x and y coordinates
 * can be set to NULL. For verification the d part can be set to NULL.
 * For Eddsa, the y part is not used and must be NULL.
 *
 * @{
 */
typedef struct cose_key {
    cose_kty_t kty;     /**< Key type */
    cose_algo_t algo;   /**< Key algorithm restriction with this key */
    cose_curve_t crv;   /**< Curve, algo is derived from this for now */
    uint8_t *kid;       /**< Key identifier */
    size_t kid_len;     /**< length of the key identifier */
    uint8_t *x;         /**< Public key part 1, must match the expected size of the algorithm */
    uint8_t *y;         /**< Public key part 2, when not NULL, must match the expected size of the algorithm */
    uint8_t *d;         /**< Private or secret key, must match the expected size of the algorithm */

    unit8_t *n;         /**< RSA modulus n per RFC 8230 Table 4 */
    unit8_t *e;         /**< RSA public exponent e per RFC 8230 Table 4 */
} cose_key_t;
/** @} */

/**
 * Initialize a cose key object, must be called before using the key
 * object
 *
 * @param   key      Signer object to initialize
 */
void cose_key_init(cose_key_t *key);

/**
 * cose_key_from_cbor initializes a key struct based on a cbor map
 *
 * @param   key      Empty key struct to fill with key information
 *
 * @return              0 on successfully loaded from cbor
 * @return              Negative on error
 */
int cose_key_from_cbor(cose_key_t *key);

/**
 * cose_key_set_key sets the key data of a key
 * Parameters according to https://tools.ietf.org/html/rfc8152#section-13
 *
 * @param   key         The key to set the key data for
 * @param   curve       The curve used
 * @param   algo        The algo to use
 * @param   x           Pointer to the "x-coordinate" of the key
 * @param   y           Pointer to the "y-coordinate" of the key
 * @param   d           Pointer to the private part of the key
 */
void cose_key_set_keys(cose_key_t *key, cose_curve_t curve, cose_algo_t algo,
                          uint8_t *x, uint8_t *y, uint8_t *d);


/**
 * Set the KID value of a key
 *
 * @param   key  The key to set the key ID for
 * @param   kid     Pointer to the key ID byte string
 * @param   len     Length of the key ID
 */
void cose_key_set_kid(cose_key_t *key, uint8_t *kid, size_t len);

/**
 * Add the protected headers to the provided CBOR map
 *
 * @param   key  The key object
 * @param   map     The cbor encoder map object to add headers to
 *
 * @return          0 on success
 * @return          Negative on error
 */
void cose_key_protected_to_map(const cose_key_t *key, nanocbor_encoder_t *map);

/**
 * Add the unprotected header information to the provided CBOR map
 *
 * @param   key  The key object
 * @param   map     The cbor encoder map object to add headers to
 *
 * @return          0 on success
 * @return          Negative on error
 */
void cose_key_unprotected_to_map(const cose_key_t *key, nanocbor_encoder_t *map);

#ifdef __cplusplus
}
#endif

#endif
/** @} */
