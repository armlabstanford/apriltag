#ifndef APRILTAGS_TAGFAMILY_H_
#define APRILTAGS_TAGFAMILY_H_

#include <climits>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <map>

#include "AprilTags/TagCodes.h"
#include "AprilTags/TagDetection.h"

namespace AprilTags {

//! Generic class for all tag encoding families
class TagFamily {
 public:
  //! The codes array is not copied internally and so must not be modified
  // externally.
  TagFamily(const TagCodes& tag_codes);

  void setErrorRecoveryBits(int b);

  void setErrorRecoveryFraction(float v);

  /* if the bits in w were arranged in a d*d grid and that grid was
   * rotated, what would the new bits in w be?
   * The bits are organized like this (for d = 3):
   *
   *  8 7 6       2 5 8      0 1 2
   *  5 4 3  ==>  1 4 7 ==>  3 4 5    (rotate90 applied twice)
   *  2 1 0       0 3 6      6 7 8
   */
  static code_t rotate90(code_t w, int d);

  //! Computes the hamming distance between two code_ts.
  static int hammingDistance(code_t a, code_t b);

  //! How many bits are set in the code_t?
  static unsigned char popCountReal(code_t w);

  static int popCount(code_t w);

  //! Given an observed tag with code 'rCode', try to recover the id.
  /*  The corresponding fields of TagDetection will be filled in. */
  void decode(TagDetection& det, code_t rCode) const;

  void printHammingDistances() const;
  //! Numer of pixels wide of the inner black border.
  //  int blackBorder;

  //! Number of bits in the tag. Must be n^2.
  int payload_bits_;

  //! Dimension of tag. e.g. for 16 bits, dimension=4. Must be sqrt(bits).
  int dimension_bits_;

  //! Minimum hamming distance between any two codes.
  /*  Accounting for rotational ambiguity? The code can recover
   *  (minHammingDistance-1)/2 bit errors.
   */
  int min_hamming_distance_;

  /* The error recovery value determines our position on the ROC
   * curve. We will report codes that are within errorRecoveryBits
   * of a valid code. Small values mean greater rejection of bogus
   * tags (but false negatives). Large values mean aggressive
   * reporting of bad tags (but with a corresponding increase in
   * false positives).
   */
  int error_recovery_bits_;

  //! The array of the codes. The id for a code is its index.
  std::vector<code_t> codes;

  static const int popCountTableShift = 12;
  static const unsigned int popCountTableSize = 1 << popCountTableShift;
  static unsigned char popCountTable[popCountTableSize];

  //! Initializes the static popCountTable
  static class TableInitializer {
   public:
    TableInitializer() {
      for (unsigned int i = 0; i < TagFamily::popCountTableSize; i++)
        TagFamily::popCountTable[i] = TagFamily::popCountReal(i);
    }
  } initializer;
};

}  // namespace AprilTags

#endif  // APRILTAGS_TAGFAMILY_H_
