{ This file is an automated translation to pascal of pix4.c                   }
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
 *  pix4.c
 *
 *    This file has these operations:
 *
 *      (1) Pixel histograms
 *      (2) Pixel row/column statistics
 *      (3) Foreground/background estimation
 *
 *    Pixel histogram, rank val, averaging and min/max
 *           NUMA       *pixGetGrayHistogram()
 *           NUMA       *pixGetGrayHistogramMasked()
 *           NUMA       *pixGetGrayHistogramInRect()
 *           l_int32     pixGetColorHistogram()
 *           l_int32     pixGetColorHistogramMasked()
 *           NUMA       *pixGetCmapHistogram()
 *           NUMA       *pixGetCmapHistogramMasked()
 *           NUMA       *pixGetCmapHistogramInRect()
 *           l_int32     pixGetRankValue()
 *           l_int32     pixGetRankValueMaskedRGB()
 *           l_int32     pixGetRankValueMasked()
 *           l_int32     pixGetAverageValue()
 *           l_int32     pixGetAverageMaskedRGB()
 *           l_int32     pixGetAverageMasked()
 *           l_int32     pixGetAverageTiledRGB()
 *           PIX        *pixGetAverageTiled()
 *           NUMA       *pixRowStats()
 *           NUMA       *pixColumnStats()
 *           l_int32     pixGetComponentRange()
 *           l_int32     pixGetExtremeValue()
 *           l_int32     pixGetMaxValueInRect()
 *           l_int32     pixGetBinnedComponentRange()
 *           l_int32     pixGetRankColorArray()
 *           l_int32     pixGetBinnedColor()
 *           PIX        *pixDisplayColorArray()
 *
 *    Pixelwise aligned statistics
 *           PIX        *pixaGetAlignedStats()
 *           l_int32     pixaExtractColumnFromEachPix()
 *           l_int32     pixGetRowStats()
 *           l_int32     pixGetColumnStats()
 *           l_int32     pixSetPixelColumn()
 *
 *    Foreground/background estimation
 *           l_int32     pixThresholdForFgBg()
 *           l_int32     pixSplitDistributionFgBg()
 *}

{*------------------------------------------------------------------*
 *                  Pixel histogram and averaging                   *
 *------------------------------------------------------------------*}

{*!
 *  pixGetGrayHistogram()
 *
 *      Input:  pixs (1, 2, 4, 8, 16 bpp; can be colormapped)
 *              factor (subsampling factor; integer >= 1)
 *      Return: na (histogram), or null on error
 *
 *  Notes:
 *      (1) If pixs has a colormap, it is converted to 8 bpp gray.
 *          If you want a histogram of the colormap indices, use
 *          pixGetCmapHistogram().
 *      (2) If pixs does not have a colormap, the output histogram is
 *          of size 2^d, where d is the depth of pixs.
 *      (3) This always returns a 256-value histogram of pixel values.
 *      (4) Set the subsampling factor > 1 to reduce the amount of computation.
 *}
function pixGetGrayHistogram( pixs: PLPix; factor: LongInt ) : PNumArray; cdecl; external LIBLEPT;


{*!
 *  pixGetGrayHistogramMasked()
 *
 *      Input:  pixs (8 bpp, or colormapped)
 *              pixm (<optional> 1 bpp mask over which histogram is
 *                    to be computed; use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0; these values are ignored if pixm is null)
 *              factor (subsampling factor; integer >= 1)
 *      Return: na (histogram), or null on error
 *
 *  Notes:
 *      (1) If pixs is cmapped, it is converted to 8 bpp gray.
 *          If you want a histogram of the colormap indices, use
 *          pixGetCmapHistogramMasked().
 *      (2) This always returns a 256-value histogram of pixel values.
 *      (3) Set the subsampling factor > 1 to reduce the amount of computation.
 *      (4) Clipping of pixm (if it exists) to pixs is done in the inner loop.
 *      (5) Input x,y are ignored unless pixm exists.
 *}
function pixGetGrayHistogramMasked( pixs, pixm: PLPix; x, y, factor: LongInt ) : PNumArray; cdecl; external LIBLEPT;


{*!
 *  pixGetGrayHistogramInRect()
 *
 *      Input:  pixs (8 bpp, or colormapped)
 *              box (<optional>) over which histogram is to be computed;
 *                   use full image if null)
 *              factor (subsampling factor; integer >= 1)
 *      Return: na (histogram), or null on error
 *
 *  Notes:
 *      (1) If pixs is cmapped, it is converted to 8 bpp gray.
 *          If you want a histogram of the colormap indices, use
 *          pixGetCmapHistogramInRect().
 *      (2) This always returns a 256-value histogram of pixel values.
 *      (3) Set the subsampling @factor > 1 to reduce the amount of computation.
 *}
function pixGetGrayHistogramInRect( pixs: PLPix; box: PLBox; factor: LongInt ) : PNumArray; cdecl; external LIBLEPT;


{*!
 *  pixGetColorHistogram()
 *
 *      Input:  pixs (rgb or colormapped)
 *              factor (subsampling factor; integer >= 1)
 *              &nar (<return> red histogram)
 *              &nag (<return> green histogram)
 *              &nab (<return> blue histogram)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) This generates a set of three 256 entry histograms,
 *          one for each color component (r,g,b).
 *      (2) Set the subsampling @factor > 1 to reduce the amount of computation.
 *}
function pixGetColorHistogram( pixs: PLPix; factor: LongInt; pnar, pnag, pnab: PPNumArray ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetColorHistogramMasked()
 *
 *      Input:  pixs (32 bpp rgb, or colormapped)
 *              pixm (<optional> 1 bpp mask over which histogram is
 *                    to be computed; use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0; these values are ignored if pixm is null)
 *              factor (subsampling factor; integer >= 1)
 *              &nar (<return> red histogram)
 *              &nag (<return> green histogram)
 *              &nab (<return> blue histogram)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) This generates a set of three 256 entry histograms,
 *      (2) Set the subsampling @factor > 1 to reduce the amount of computation.
 *      (3) Clipping of pixm (if it exists) to pixs is done in the inner loop.
 *      (4) Input x,y are ignored unless pixm exists.
 *}
function pixGetColorHistogramMasked( pixs, pixm: PLPix; x, y, factor: LongInt; pnar, pnag, pnab: PPNumArray ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetCmapHistogram()
 *
 *      Input:  pixs (colormapped: d = 2, 4 or 8)
 *              factor (subsampling factor; integer >= 1)
 *      Return: na (histogram of cmap indices), or null on error
 *
 *  Notes:
 *      (1) This generates a histogram of colormap pixel indices,
 *          and is of size 2^d.
 *      (2) Set the subsampling @factor > 1 to reduce the amount of computation.
 *}
function pixGetCmapHistogram( pixs: PLPix; factor: LongInt ) : PNumArray; cdecl; external LIBLEPT;


{*!
 *  pixGetCmapHistogramMasked()
 *
 *      Input:  pixs (colormapped: d = 2, 4 or 8)
 *              pixm (<optional> 1 bpp mask over which histogram is
 *                    to be computed; use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0; these values are ignored if pixm is null)
 *              factor (subsampling factor; integer >= 1)
 *      Return: na (histogram), or null on error
 *
 *  Notes:
 *      (1) This generates a histogram of colormap pixel indices,
 *          and is of size 2^d.
 *      (2) Set the subsampling @factor > 1 to reduce the amount of computation.
 *      (3) Clipping of pixm to pixs is done in the inner loop.
 *}
function pixGetCmapHistogramMasked( pixs, pixm: PLPix; x, y, factor: LongInt ) : PNumArray; cdecl; external LIBLEPT;


{*!
 *  pixGetCmapHistogramInRect()
 *
 *      Input:  pixs (colormapped: d = 2, 4 or 8)
 *              box (<optional>) over which histogram is to be computed;
 *                   use full image if null)
 *              factor (subsampling factor; integer >= 1)
 *      Return: na (histogram), or null on error
 *
 *  Notes:
 *      (1) This generates a histogram of colormap pixel indices,
 *          and is of size 2^d.
 *      (2) Set the subsampling @factor > 1 to reduce the amount of computation.
 *      (3) Clipping to the box is done in the inner loop.
 *}
function pixGetCmapHistogramInRect( pixs: PLPix; box: PLBox; factor: LongInt ) : PNumArray; cdecl; external LIBLEPT;


{*!
 *  pixGetRankValue()
 *
 *      Input:  pixs (8 bpp, 32 bpp or colormapped)
 *              factor (subsampling factor; integer >= 1)
 *              rank (between 0.0 and 1.0; 1.0 is brightest, 0.0 is darkest)
 *              &value (<return> pixel value corresponding to input rank)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) Simple function to get rank values of an image.
 *          For a color image, the median value (rank = 0.5) can be
 *          used to linearly remap the colors based on the median
 *          of a target image, using pixLinearMapToTargetColor().
 *}
function pixGetRankValue( pixs: PLPix; factor: LongInt; rank: Single; pvalue: Pointer ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetRankValueMaskedRGB()
 *
 *      Input:  pixs (32 bpp)
 *              pixm (<optional> 1 bpp mask over which rank val is to be taken;
 *                    use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0; these values are ignored if pixm is null)
 *              factor (subsampling factor; integer >= 1)
 *              rank (between 0.0 and 1.0; 1.0 is brightest, 0.0 is darkest)
 *              &rval (<optional return> red component val for to input rank)
 *              &gval (<optional return> green component val for to input rank)
 *              &bval (<optional return> blue component val for to input rank)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) Computes the rank component values of pixels in pixs that
 *          are under the fg of the optional mask.  If the mask is null, it
 *          computes the average of the pixels in pixs.
 *      (2) Set the subsampling @factor > 1 to reduce the amount of
 *          computation.
 *      (4) Input x,y are ignored unless pixm exists.
 *      (5) The rank must be in [0.0 ... 1.0], where the brightest pixel
 *          has rank 1.0.  For the median pixel value, use 0.5.
 *}
function pixGetRankValueMaskedRGB( pixs, pixm: PLPix; x, y, factor: LongInt; rank: Single; prval, pgval, pbval: PSingle ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetRankValueMasked()
 *
 *      Input:  pixs (8 bpp, or colormapped)
 *              pixm (<optional> 1 bpp mask over which rank val is to be taken;
 *                    use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0; these values are ignored if pixm is null)
 *              factor (subsampling factor; integer >= 1)
 *              rank (between 0.0 and 1.0; 1.0 is brightest, 0.0 is darkest)
 *              &val (<return> pixel value corresponding to input rank)
 *              &na (<optional return> of histogram)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) Computes the rank value of pixels in pixs that are under
 *          the fg of the optional mask.  If the mask is null, it
 *          computes the average of the pixels in pixs.
 *      (2) Set the subsampling @factor > 1 to reduce the amount of
 *          computation.
 *      (3) Clipping of pixm (if it exists) to pixs is done in the inner loop.
 *      (4) Input x,y are ignored unless pixm exists.
 *      (5) The rank must be in [0.0 ... 1.0], where the brightest pixel
 *          has rank 1.0.  For the median pixel value, use 0.5.
 *      (6) The histogram can optionally be returned, so that other rank
 *          values can be extracted without recomputing the histogram.
 *          In that case, just use
 *              numaHistogramGetValFromRank(na, rank, &val);
 *          on the returned Numa for additional rank values.
 *}
function pixGetRankValueMasked( pixs, pixm: PLPix; x, y, factor: LongInt; rank: Single; pval: PSingle; pna: PPNumArray ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetAverageValue()
 *
 *      Input:  pixs (8 bpp, 32 bpp or colormapped)
 *              factor (subsampling factor; integer >= 1)
 *              type (L_MEAN_ABSVAL, L_ROOT_MEAN_SQUARE,
 *                    L_STANDARD_DEVIATION, L_VARIANCE)
 *              &value (<return> pixel value corresponding to input rank)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) Simple function to get average statistical values of an image.
 *}
function pixGetAverageValue( pixs: PLPix; factor, L_type: LongInt; pvalue: Pointer ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetAverageMaskedRGB()
 *
 *      Input:  pixs (32 bpp, or colormapped)
 *              pixm (<optional> 1 bpp mask over which average is to be taken;
 *                    use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0)
 *              factor (subsampling factor; >= 1)
 *              type (L_MEAN_ABSVAL, L_ROOT_MEAN_SQUARE,
 *                    L_STANDARD_DEVIATION, L_VARIANCE)
 *              &rval (<return optional> measured red value of given 'type')
 *              &gval (<return optional> measured green value of given 'type')
 *              &bval (<return optional> measured blue value of given 'type')
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) For usage, see pixGetAverageMasked().
 *      (2) If there is a colormap, it is removed before the 8 bpp
 *          component images are extracted.
 *}
function pixGetAverageMaskedRGB( pixs, pixm: PLPix; x, y, factor, L_type: LongInt; prval, pgval, pbval: PSingle ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetAverageMasked()
 *
 *      Input:  pixs (8 or 16 bpp, or colormapped)
 *              pixm (<optional> 1 bpp mask over which average is to be taken;
 *                    use all pixels if null)
 *              x, y (UL corner of pixm relative to the UL corner of pixs;
 *                    can be < 0)
 *              factor (subsampling factor; >= 1)
 *              type (L_MEAN_ABSVAL, L_ROOT_MEAN_SQUARE,
 *                    L_STANDARD_DEVIATION, L_VARIANCE)
 *              &val (<return> measured value of given 'type')
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) Use L_MEAN_ABSVAL to get the average value of pixels in pixs
 *          that are under the fg of the optional mask.  If the mask
 *          is null, it finds the average of the pixels in pixs.
 *      (2) Likewise, use L_ROOT_MEAN_SQUARE to get the rms value of
 *          pixels in pixs, either masked or not; L_STANDARD_DEVIATION
 *          to get the standard deviation from the mean of the pixels;
 *          L_VARIANCE to get the average squared difference from the
 *          expected value.  The variance is the square of the stdev.
 *          For the standard deviation, we use
 *              sqrt(<(<x> - x)>^2) = sqrt(<x^2> - <x>^2)
 *      (3) Set the subsampling @factor > 1 to reduce the amount of
 *          computation.
 *      (4) Clipping of pixm (if it exists) to pixs is done in the inner loop.
 *      (5) Input x,y are ignored unless pixm exists.
 *}
function pixGetAverageMasked( pixs, pixm: PLPix; x, y, factor, L_type: LongInt; pval: PSingle ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetAverageTiledRGB()
 *
 *      Input:  pixs (32 bpp, or colormapped)
 *              sx, sy (tile size; must be at least 2 x 2)
 *              type (L_MEAN_ABSVAL, L_ROOT_MEAN_SQUARE, L_STANDARD_DEVIATION)
 *              &pixr (<optional return> tiled 'average' of red component)
 *              &pixg (<optional return> tiled 'average' of green component)
 *              &pixb (<optional return> tiled 'average' of blue component)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) For usage, see pixGetAverageTiled().
 *      (2) If there is a colormap, it is removed before the 8 bpp
 *          component images are extracted.
 *}
function pixGetAverageTiledRGB( pixs: PLPix; sx, sy, L_type: LongInt; ppixr, ppixg, ppixb: PPLPix ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetAverageTiled()
 *
 *      Input:  pixs (8 bpp, or colormapped)
 *              sx, sy (tile size; must be at least 2 x 2)
 *              type (L_MEAN_ABSVAL, L_ROOT_MEAN_SQUARE, L_STANDARD_DEVIATION)
 *      Return: pixd (average values in each tile), or null on error
 *
 *  Notes:
 *      (1) Only computes for tiles that are entirely contained in pixs.
 *      (2) Use L_MEAN_ABSVAL to get the average abs value within the tile;
 *          L_ROOT_MEAN_SQUARE to get the rms value within each tile;
 *          L_STANDARD_DEVIATION to get the standard dev. from the average
 *          within each tile.
 *      (3) If colormapped, converts to 8 bpp gray.
 *}
function pixGetAverageTiled( pixs: PLPix; sx, sy, L_type: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*!
 *  pixRowStats()
 *
 *      Input:  pixs (8 bpp; not cmapped)
 *              &namean (<optional return> numa of mean values)
 *              &namedian (<optional return> numa of median values)
 *              &namode (<optional return> numa of mode intensity values)
 *              &namodecount (<optional return> numa of mode counts)
 *              &navar (<optional return> numa of variance)
 *              &narootvar (<optional return> numa of square root of variance)
 *      Return: na (numa of requested statistic for each row), or null on error
 *
 *  Notes:
 *      (1) This computes numas that represent column vectors of statistics,
 *          with each of its values derived from the corresponding row of a Pix.
 *      (2) Use NULL on input to prevent computation of any of the 5 numas.
 *      (3) Other functions that compute pixel row statistics are:
 *             pixCountPixelsByRow()
 *             pixSumPixelsByRow()
 *             pixGetRowStats()
 *}
function pixRowStats( pixs: PLPix; pnamean, pnamedian, pnamode, pnamodecount, pnavar, pnarootvar: PPNumArray ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixColumnStats()
 *
 *      Input:  pixs (8 bpp; not cmapped)
 *              &namean (<optional return> numa of mean values)
 *              &namedian (<optional return> numa of median values)
 *              &namode (<optional return> numa of mode intensity values)
 *              &namodecount (<optional return> numa of mode counts)
 *              &navar (<optional return> numa of variance)
 *              &narootvar (<optional return> numa of square root of variance)
 *      Return: na (numa of requested statistic for each column),
 *                  or null on error
 *
 *  Notes:
 *      (1) This computes numas that represent row vectors of statistics,
 *          with each of its values derived from the corresponding col of a Pix.
 *      (2) Use NULL on input to prevent computation of any of the 5 numas.
 *      (3) Other functions that compute pixel column statistics are:
 *             pixCountPixelsByColumn()
 *             pixSumPixelsByColumn()
 *             pixGetColumnStats()
 *}
function pixColumnStats( pixs: PLPix; pnamean, pnamedian, pnamode, pnamodecount, pnavar, pnarootvar: PPNumArray ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetComponentRange()
 *
 *      Input:  pixs (8 bpp grayscale, 32 bpp rgb, or colormapped)
 *              factor (subsampling factor; >= 1; ignored if colormapped)
 *              color (L_SELECT_RED, L_SELECT_GREEN or L_SELECT_BLUE)
 *              &minval (<optional return> minimum value of component)
 *              &maxval (<optional return> maximum value of component)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) If pixs is 8 bpp grayscale, the color selection type is ignored.
 *}
function pixGetComponentRange( pixs: PLPix; factor, color: LongInt; pminval, pmaxval: PLongint ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetExtremeValue()
 *
 *      Input:  pixs (8 bpp grayscale, 32 bpp rgb, or colormapped)
 *              factor (subsampling factor; >= 1; ignored if colormapped)
 *              type (L_SELECT_MIN or L_SELECT_MAX)
 *              &rval (<optional return> red component)
 *              &gval (<optional return> green component)
 *              &bval (<optional return> blue component)
 *              &grayval (<optional return> min gray value)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) If pixs is grayscale, the result is returned in &grayval.
 *          Otherwise, if there is a colormap or d == 32,
 *          each requested color component is returned.  At least
 *          one color component (address) must be input.
 *}
function pixGetExtremeValue( pixs: PLPix; factor, L_type: LongInt; prval, pgval, pbval, pgrayval: PLongint ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetMaxValueInRect()
 *
 *      Input:  pixs (8 bpp or 32 bpp grayscale; no color space components)
 *              box (<optional> region; set box = NULL to use entire pixs)
 *              &maxval (<optional return> max value in region)
 *              &xmax (<optional return> x location of max value)
 *              &ymax (<optional return> y location of max value)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) This can be used to find the maximum and its location
 *          in a 2-dimensional histogram, where the x and y directions
 *          represent two color components (e.g., saturation and hue).
 *      (2) Note that here a 32 bpp pixs has pixel values that are simply
 *          numbers.  They are not 8 bpp components in a colorspace.
 *}
function pixGetMaxValueInRect( pixs: PLPix; box: PLBox; pmaxval: Pointer; pxmax, pymax: PLongint ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetBinnedComponentRange()
 *
 *      Input:  pixs (32 bpp rgb)
 *              nbins (number of equal population bins; must be > 1)
 *              factor (subsampling factor; >= 1)
 *              color (L_SELECT_RED, L_SELECT_GREEN or L_SELECT_BLUE)
 *              &minval (<optional return> minimum value of component)
 *              &maxval (<optional return> maximum value of component)
 *              &carray (<optional return> color array of bins)
 *              debugflag (1 for debug output)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) This returns the min and max average values of the
 *          selected color component in the set of rank bins,
 *          where the ranking is done using the specified component.
 *}
function pixGetBinnedComponentRange( pixs: PLPix; nbins, factor, color: LongInt; pminval, pmaxval: PLongint; pcarray: PPLongword; debugflag: LongInt ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetRankColorArray()
 *
 *      Input:  pixs (32 bpp or cmapped)
 *              nbins (number of equal population bins; must be > 1)
 *              type (color selection flag)
 *              factor (subsampling factor; integer >= 1)
 *              &carray (<return> array of colors, ranked by intensity)
 *              debugflag (1 to display color squares and plots of color
 *                         components; 2 to write them as png to file)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) The color selection flag is one of: L_SELECT_RED, L_SELECT_GREEN,
 *          L_SELECT_BLUE, L_SELECT_MIN, L_SELECT_MAX.
 *      (2) Then it finds the histogram of the selected component in each
 *          RGB pixel.  For each of the @nbins sets of pixels,
 *          ordered by this component value, find the average color,
 *          and return this as a "rank color" array.  The output array
 *          has @nbins colors.
 *      (3) Set the subsampling factor > 1 to reduce the amount of
 *          computation.  Typically you want at least 10,000 pixels
 *          for reasonable statistics.
 *      (4) The rank color as a function of rank can then be found from
 *             rankint = (l_int32)(rank * (nbins - 1) + 0.5);
 *             extractRGBValues(array[rankint], &rval, &gval, &bval);
 *          where the rank is in [0.0 ... 1.0].
 *          This function is meant to be simple and approximate.
 *      (5) Compare this with pixGetBinnedColor(), which generates equal
 *          width intensity bins and finds the average color in each bin.
 *}
function pixGetRankColorArray( pixs: PLPix; nbins, L_type, factor: LongInt; pcarray: PPLongword; debugflag: LongInt ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetBinnedColor()
 *
 *      Input:  pixs (32 bpp)
 *              pixg (8 bpp grayscale version of pixs)
 *              factor (sampling factor along pixel counting direction)
 *              nbins (number of intensity bins)
 *              nalut (LUT for mapping from intensity to bin number)
 *              &carray (<return> array of average color values in each bin)
 *              debugflag (1 to display output debug plots of color
 *                         components; 2 to write them as png to file)
 *      Return: 0 if OK; 1 on error
 *
 *  Notes:
 *      (1) This takes a color image, a grayscale (intensity) version,
 *          a LUT from intensity to bin number, and the number of bins.
 *          It computes the average color for pixels whose intensity
 *          is in each bin.  This is returned as an array of l_uint32
 *          colors in our standard RGBA ordering.
 *      (2) This function generates equal width intensity bins and
 *          finds the average color in each bin.  Compare this with
 *          pixGetRankColorArray(), which rank orders the pixels
 *          by the value of the selected component in each pixel,
 *          sets up bins with equal population (not intensity width!),
 *          and gets the average color in each bin.
 *}
function pixGetBinnedColor( pixs, pixg: PLPix; factor, nbins: LongInt; nalut: PNumArray; pcarray: PPLongword; debugflag: LongInt ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixDisplayColorArray()
 *
 *      Input:  carray (array of colors: 0xrrggbb00)
 *              ncolors (size of array)
 *              side (size of each color square; suggest 200)
 *              ncols (number of columns in output color matrix)
 *              textflag (1 to label each square with text; 0 otherwise)
 *      Return: pixd (color array), or null on error
 *}
function pixDisplayColorArray( carray: Pointer; ncolors, side, ncols, textflag: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*-------------------------------------------------------------*
 *                 Pixelwise aligned statistics                *
 *-------------------------------------------------------------*}

{*!
 *  pixaGetAlignedStats()
 *
 *      Input:  pixa (of identically sized, 8 bpp pix; not cmapped)
 *              type (L_MEAN_ABSVAL, L_MEDIAN_VAL, L_MODE_VAL, L_MODE_COUNT)
 *              nbins (of histogram for median and mode; ignored for mean)
 *              thresh (on histogram for mode val; ignored for all other types)
 *      Return: pix (with pixelwise aligned stats), or null on error.
 *
 *  Notes:
 *      (1) Each pixel in the returned pix represents an average
 *          (or median, or mode) over the corresponding pixels in each
 *          pix in the pixa.
 *      (2) The @thresh parameter works with L_MODE_VAL only, and
 *          sets a minimum occupancy of the mode bin.
 *          If the occupancy of the mode bin is less than @thresh, the
 *          mode value is returned as 0.  To always return the actual
 *          mode value, set @thresh = 0.  See pixGetRowStats().
 *}
function pixaGetAlignedStats( pixa: PPixArray; L_type, nbins, thresh: LongInt ) : PLPix; cdecl; external LIBLEPT;


{*!
 *  pixaExtractColumnFromEachPix()
 *
 *      Input:  pixa (of identically sized, 8 bpp; not cmapped)
 *              col (column index)
 *              pixd (pix into which each column is inserted)
 *      Return: 0 if OK, 1 on error
 *}
function pixaExtractColumnFromEachPix( pixa: PPixArray; col: LongInt; pixd: PLPix ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetRowStats()
 *
 *      Input:  pixs (8 bpp; not cmapped)
 *              type (L_MEAN_ABSVAL, L_MEDIAN_VAL, L_MODE_VAL, L_MODE_COUNT)
 *              nbins (of histogram for median and mode; ignored for mean)
 *              thresh (on histogram for mode; ignored for mean and median)
 *              colvect (vector of results gathered across the rows of pixs)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) This computes a column vector of statistics using each
 *          row of a Pix.  The result is put in @colvect.
 *      (2) The @thresh parameter works with L_MODE_VAL only, and
 *          sets a minimum occupancy of the mode bin.
 *          If the occupancy of the mode bin is less than @thresh, the
 *          mode value is returned as 0.  To always return the actual
 *          mode value, set @thresh = 0.
 *      (3) What is the meaning of this @thresh parameter?
 *          For each row, the total count in the histogram is w, the
 *          image width.  So @thresh, relative to w, gives a measure
 *          of the ratio of the bin width to the width of the distribution.
 *          The larger @thresh, the narrower the distribution must be
 *          for the mode value to be returned (instead of returning 0).
 *      (4) If the Pix consists of a set of corresponding columns,
 *          one for each Pix in a Pixa, the width of the Pix is the
 *          number of Pix in the Pixa and the column vector can
 *          be stored as a column in a Pix of the same size as
 *          each Pix in the Pixa.
 *}
function pixGetRowStats( pixs: PLPix; L_type, nbins, thresh: LongInt; colvect: PSingle ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixGetColumnStats()
 *
 *      Input:  pixs (8 bpp; not cmapped)
 *              type (L_MEAN_ABSVAL, L_MEDIAN_VAL, L_MODE_VAL, L_MODE_COUNT)
 *              nbins (of histogram for median and mode; ignored for mean)
 *              thresh (on histogram for mode val; ignored for all other types)
 *              rowvect (vector of results gathered down the columns of pixs)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) This computes a row vector of statistics using each
 *          column of a Pix.  The result is put in @rowvect.
 *      (2) The @thresh parameter works with L_MODE_VAL only, and
 *          sets a minimum occupancy of the mode bin.
 *          If the occupancy of the mode bin is less than @thresh, the
 *          mode value is returned as 0.  To always return the actual
 *          mode value, set @thresh = 0.
 *      (3) What is the meaning of this @thresh parameter?
 *          For each column, the total count in the histogram is h, the
 *          image height.  So @thresh, relative to h, gives a measure
 *          of the ratio of the bin width to the width of the distribution.
 *          The larger @thresh, the narrower the distribution must be
 *          for the mode value to be returned (instead of returning 0).
 *}
function pixGetColumnStats( pixs: PLPix; L_type, nbins, thresh: LongInt; rowvect: PSingle ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixSetPixelColumn()
 *
 *      Input:  pix (8 bpp; not cmapped)
 *              col (column index)
 *              colvect (vector of floats)
 *      Return: 0 if OK, 1 on error
 *}
function pixSetPixelColumn( pix: PLPix; col: LongInt; colvect: PSingle ) : LongInt; cdecl; external LIBLEPT;


{*-------------------------------------------------------------*
 *              Foreground/background estimation               *
 *-------------------------------------------------------------*}

{*!
 *  pixThresholdForFgBg()
 *
 *      Input:  pixs (any depth; cmapped ok)
 *              factor (subsampling factor; integer >= 1)
 *              thresh (threshold for generating foreground mask)
 *              &fgval (<optional return> average foreground value)
 *              &bgval (<optional return> average background value)
 *      Return: 0 if OK, 1 on error
 *}
function pixThresholdForFgBg( pixs: PLPix; factor, thresh: LongInt; pfgval, pbgval: PLongint ) : LongInt; cdecl; external LIBLEPT;


{*!
 *  pixSplitDistributionFgBg()
 *
 *      Input:  pixs (any depth; cmapped ok)
 *              scorefract (fraction of the max score, used to determine
 *                          the range over which the histogram min is searched)
 *              factor (subsampling factor; integer >= 1)
 *              &thresh (<optional return> best threshold for separating)
 *              &fgval (<optional return> average foreground value)
 *              &bgval (<optional return> average background value)
 *              debugflag (1 for plotting of distribution and split point)
 *      Return: 0 if OK, 1 on error
 *
 *  Notes:
 *      (1) See numaSplitDistribution() for details on the underlying
 *          method of choosing a threshold.
 *}
function pixSplitDistributionFgBg( pixs: PLPix; scorefract: Single; factor: LongInt; pthresh, pfgval, pbgval: PLongint; debugflag: LongInt ) : LongInt; cdecl; external LIBLEPT;


