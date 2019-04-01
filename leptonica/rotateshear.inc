{ This file is an automated translation to pascal of rotateshear.c            }
{ performed using the Leptonica Library Converter, written by Malcolm Poole   }

{*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*}

{*
 *  rotateshear.c
 *
 *      Shear rotation about arbitrary point using 2 and 3 shears
 *
 *              PIX      *pixRotateShear()
 *              PIX      *pixRotate2Shear()
 *              PIX      *pixRotate3Shear()
 *
 *      Shear rotation in-place about arbitrary point using 3 shears
 *              l_int32   pixRotateShearIP()
 *
 *      Shear rotation around the image center
 *              PIX      *pixRotateShearCenter()    (2 or 3 shears)
 *              l_int32   pixRotateShearCenterIP()  (3 shears)
 *
 *  Rotation is measured in radians; clockwise rotations are positive.
 *
 *  Rotation by shear works on images of any depth,
 *  including 8 bpp color paletted images and 32 bpp
 *  rgb images.  It works by translating each src pixel
 *  value to the appropriate pixel in the rotated dest.
 *  For 8 bpp grayscale images, it is about 10-15x faster
 *  than rotation by area-mapping.
 *
 *  This speed and flexibility comes at the following cost,
 *  relative to area-mapped rotation:
 *
 *    -  Jaggies are created on edges of straight lines
 *
 *    -  For large angles, where you must use 3 shears,
 *       there is some extra clipping from the shears.
 *
 *  For small angles, typically less than 0.05 radians,
 *  rotation can be done with 2 orthogonal shears.
 *  Two such continuous shears (as opposed to the discrete
 *  shears on a pixel lattice that we have here) give
 *  a rotated image that has a distortion in the lengths
 *  of the two rotated and still-perpendicular axes.  The
 *  length/width ratio changes by a fraction
 *
 *       0.5 * (angle)**2
 *
 *  For an angle of 0.05 radians, this is about 1 part in
 *  a thousand.  This distortion is absent when you use
 *  3 continuous shears with the correct angles (see below).
 *
 *  Of course, the image is on a discrete pixel lattice.
 *  Rotation by shear gives an approximation to a continuous
 *  rotation, leaving pixel jaggies at sharp boundaries.
 *  For very small rotations, rotating from a corner gives
 *  better sensitivity than rotating from the image center.
 *  Here's why.  Define the shear "center" to be the line such
 *  that the image is sheared in opposite directions on
 *  each side of and parallel to the line.  For small
 *  rotations there is a "dead space" on each side of the
 *  shear center of width equal to half the shear angle,
 *  in radians.  Thus, when the image is sheared about the center,
 *  the dead space width equals the shear angle, but when
 *  the image is sheared from a corner, the dead space
 *  width is only half the shear angle.
 *
 *  All horizontal and vertical shears are implemented by
 *  rasterop.  The in-place rotation uses special in-place
 *  shears that copy rows sideways or columns vertically
 *  without buffering, and then rewrite old pixels that are
 *  no longer covered by sheared pixels.  For that rewriting,
 *  you have the choice of using white or black pixels.
 *  (Note that this may give undesirable results for colormapped
 *  images, where the white and black values are arbitrary
 *  indexes into the colormap, and may not even exist.)
 *
 *  Rotation by shear is fast and depth-independent.  However, it
 *  does not work well for large rotation angles.  In fact, for
 *  rotation angles greater than about 7 degrees, more pixels are
 *  lost at the edges than when using pixRotationBySampling(), which
 *  only loses pixels because they are rotated out of the image.
 *  For large rotations, use pixRotationBySampling() or, for
 *  more accuracy when d > 1 bpp, pixRotateAM().
 *
 *  For small angles, when comparing the quality of rotation by
 *  sampling and by shear, you can see that rotation by sampling
 *  is slightly more accurate.  However, the difference in
 *  accuracy of rotation by sampling when compared to 3-shear and
 *  (for angles less than 2 degrees, when compared to 2-shear) is
 *  less than 1 pixel at any point.  For very small angles, rotation by
 *  sampling is slower than rotation by shear.  The speed difference
 *  depends on the pixel depth and the rotation angle.  Rotation
 *  by shear is very fast for small angles and for small depth (esp. 1 bpp).
 *  Rotation by sampling speed is independent of angle and relatively
 *  more efficient for 8 and 32 bpp images.  Here are some timings
 *  for the ratio of rotation times: (time for sampling)/ (time for shear)
  *
 *       depth (bpp)       ratio (2 deg)       ratio (10 deg)
 *       -----------------------------------------------------
 *          1                  25                  6
 *          8                   5                  2.6
 *          32                  1.6                1.0
 *
 *  Consequently, for small angles and low bit depth, use rotation by shear.
 *  For large angles or large bit depth, use rotation by sampling.
 *
 *  There has been some work on what is called a "quasishear
 *  rotation" ("The Quasi-Shear Rotation, Eric Andres,
 *  DGCI 1996, pp. 307-314).  I believe they use a 3-shear
 *  approximation to the continuous rotation, exactly as
 *  we do here.  The approximation is due to being on
 *  a square pixel lattice.  They also use integers to specify
 *  the rotation angle and center offset, but that makes
 *  little sense on a machine where you have a few GFLOPS
 *  and only a few hundred floating point operations to do (!)
 *  They also allow subpixel specification of the center of
 *  rotation, which I haven't bothered with, and claim that
 *  better results are possible if each of the 4 quadrants is
 *  handled separately.
 *
 *  But the bottom line is that for binary images, the quality
 *  of the simple 3-shear rotation is about as good as you can do,
 *  visually, without dithering the result.  The effect of dither
 *  is to break up the horizontal and vertical shear lines.
 *  It's a bit tricky to dither with block shears -- you have to
 *  dither the pixels on the block boundaries!
 *}
//const VERY_SMALL_ANGLE : Single = 0.001;     // radians; ~0.06 degrees 
const MAX_2_SHEAR_ANGLE : Single = 0.05;     // radians; ~3 degrees    

{*------------------------------------------------------------------*
 *                Rotations about an arbitrary point                *
 *------------------------------------------------------------------*}

{*!
 *  pixRotateShear()
 *
 *      Input:  pixs
 *              xcen (x value for which there is no horizontal shear)
 *              ycen (y value for which there is no vertical shear)
 *              angle (radians)
 *              incolor (L_BRING_IN_WHITE, L_BRING_IN_BLACK);
 *      Return: pixd, or null on error.
 *
 *  Notes:
 *      (1) This rotates an image about the given point, using
 *          either 2 or 3 shears.
 *      (2) A positive angle gives a clockwise rotation.
 *      (3) This brings in 'incolor' pixels from outside the image.
 *}
function pixRotateShear( pixs: PLPix; xcen, ycen: LongInt; angle: Single; incolor: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*!
 *  pixRotate2Shear()
 *
 *      Input:  pixs
 *              xcen, ycen (center of rotation)
 *              angle (radians)
 *              incolor (L_BRING_IN_WHITE, L_BRING_IN_BLACK);
 *      Return: pixd, or null on error.
 *
 *  Notes:
 *      (1) This rotates the image about the given point,
 *          using the 2-shear method.  It should only
 *          be used for angles smaller than MAX_2_SHEAR_ANGLE.
 *      (2) A positive angle gives a clockwise rotation.
 *      (3) 2-shear rotation by a specified angle is equivalent
 *          to the sequential transformations
 *             x' = x + tan(angle) * (y - ycen)     for x-shear
 *             y' = y + tan(angle) * (x - xcen)     for y-shear
 *      (4) Computation of tan(angle) is performed within the shear operation.
 *      (5) This brings in 'incolor' pixels from outside the image.
 *}
function pixRotate2Shear( pixs: PLPix; xcen, ycen: LongInt; angle: Single; incolor: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*!
 *  pixRotate3Shear()
 *
 *      Input:  pixs
 *              xcen, ycen (center of rotation)
 *              angle (radians)
 *              incolor (L_BRING_IN_WHITE, L_BRING_IN_BLACK);
 *      Return: pixd, or null on error.
 *
 *  Notes:
 *      (1) This rotates the image about the image center,
 *          using the 3-shear method.  It can be used for any angle, and
 *          should be used for angles larger than MAX_2_SHEAR_ANGLE.
 *      (2) A positive angle gives a clockwise rotation.
 *      (3) 3-shear rotation by a specified angle is equivalent
 *          to the sequential transformations
 *            y' = y + tan(angle/2) * (x - xcen)     for first y-shear
 *            x' = x + sin(angle) * (y - ycen)       for x-shear
 *            y' = y + tan(angle/2) * (x - xcen)     for second y-shear
 *      (4) Computation of tan(angle) is performed in the shear operations.
 *      (5) This brings in 'incolor' pixels from outside the image.
 *      (6) The algorithm was published by Alan Paeth: "A Fast Algorithm
 *          for General Raster Rotation," Graphics Interface '86,
 *          pp. 77-81, May 1986.  A description of the method, along with
 *          an implementation, can be found in Graphics Gems, p. 179,
 *          edited by Andrew Glassner, published by Academic Press, 1990.
 *}
function pixRotate3Shear( pixs: PLPix; xcen, ycen: LongInt; angle: Single; incolor: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*------------------------------------------------------------------*
 *             Rotations in-place about an arbitrary point          *
 *------------------------------------------------------------------*}

{*!
 *  pixRotateShearIP()
 *
 *      Input:  pixs (any depth; not colormapped)
 *              xcen, ycen (center of rotation)
 *              angle (radians)
 *              incolor (L_BRING_IN_WHITE, L_BRING_IN_BLACK)
 *      Return: 0 if OK; 1 on error
 *
 *  Notes:
 *      (1) This does an in-place rotation of the image
 *          about the image center, using the 3-shear method.
 *      (2) A positive angle gives a clockwise rotation.
 *      (3) 3-shear rotation by a specified angle is equivalent
 *          to the sequential transformations
 *            y' = y + tan(angle/2) * (x - xcen)      for first y-shear
 *            x' = x + sin(angle) * (y - ycen)        for x-shear
 *            y' = y + tan(angle/2) * (x - xcen)      for second y-shear
 *      (4) Computation of tan(angle) is performed in the shear operations.
 *      (5) This brings in 'incolor' pixels from outside the image.
 *      (6) The pix cannot be colormapped, because the in-place operation
 *          only blits in 0 or 1 bits, not an arbitrary colormap index.
 *}
function pixRotateShearIP( pixs: PLPix; xcen, ycen: LongInt; angle: Single; incolor: LongInt ) : LongInt; cdecl; external LIBLEPT;


{*------------------------------------------------------------------*
 *                    Rotations about the image center              *
 *------------------------------------------------------------------*}

{*!
 *  pixRotateShearCenter()
 *
 *      Input:  pixs
 *              angle (radians)
 *              incolor (L_BRING_IN_WHITE, L_BRING_IN_BLACK)
 *      Return: pixd, or null on error
 *}
function pixRotateShearCenter( pixs: PLPix; angle: Single; incolor: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*!
 *  pixRotateShearCenterIP()
 *
 *      Input:  pixs
 *              angle (radians)
 *              incolor (L_BRING_IN_WHITE, L_BRING_IN_BLACK)
 *      Return: 0 if OK, 1 on error
 *}
function pixRotateShearCenterIP( pixs: PLPix; angle: Single; incolor: LongInt ) : LongInt; cdecl; external LIBLEPT;

