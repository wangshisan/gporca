//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal, Inc.
//
//	@filename:
//		CJoinStatsProcessor.h
//
//	@doc:
//		Compute statistics for all joins
//---------------------------------------------------------------------------
#ifndef GPNAUCRATES_CJoinStatsProcessor_H
#define GPNAUCRATES_CJoinStatsProcessor_H

#include "gpopt/operators/ops.h"
#include "gpopt/optimizer/COptimizerConfig.h"

#include "naucrates/statistics/CStatistics.h"
#include "naucrates/statistics/CGroupByStatsProcessor.h"
#include "naucrates/statistics/CStatisticsUtils.h"

namespace gpnaucrates
{

	// Parent class for computing statistics for all joins
	class CJoinStatsProcessor
	{
		protected:

			// return join cardinality based on scaling factor and join type
			static
			CDouble CalcJoinCardinality
				(
				 CStatisticsConfig *stats_config,
				 CDouble left_num_rows,
				 CDouble right_num_rows,
				 CDoubleArray *join_conds_scale_factors,
				 IStatistics::EStatsJoinType join_type
				);


			// check if the join statistics object is empty output based on the input
			// histograms and the join histograms
			static
			BOOL JoinStatsAreEmpty
				(
				 BOOL outer_is_empty,
				 BOOL output_is_empty,
				 const CHistogram *outer_histogram,
				 const CHistogram *inner_histogram,
				 CHistogram *join_histogram,
				 IStatistics::EStatsJoinType join_type
				 );

			// helper for joining histograms
			static
			void JoinHistograms
					(
							IMemoryPool *mp,
							const CHistogram *histogram1,
							const CHistogram *histogram2,
							CStatsPredJoin *join_pred_stats,
							CDouble num_rows1,
							CDouble num_rows2,
							CHistogram **result_hist1, // output: histogram 1 after join
							CHistogram **result_hist2, // output: histogram 2 after join
							CDouble *scale_factor, // output: scale factor based on the join
							BOOL is_input_empty, // if true, one of the inputs is empty
							IStatistics::EStatsJoinType join_type,
							BOOL DoIgnoreLASJHistComputation
					);

		public:

			// main driver to generate join stats
			static
			CStatistics *SetResultingJoinStats
				(
				 IMemoryPool *mp,
				 CStatisticsConfig *stats_config,
				 const IStatistics *outer_stats_input,
				 const IStatistics *inner_stats_input,
												  CStatsPredJoinArray *join_preds_stats,
				 IStatistics::EStatsJoinType join_type,
				 BOOL DoIgnoreLASJHistComputation
				 );

			static
			IStatistics *CalcAllJoinStats
				(
				 IMemoryPool *mp,
				 IStatisticsArray *statistics_array,
				 CExpression *expr,
				 IStatistics::EStatsJoinType join_type
				 );

			// derive statistics for join operation given array of statistics object
			static
			IStatistics *DeriveJoinStats
				(
				 IMemoryPool *mp,
				 CExpressionHandle &exprhdl,
				 IStatisticsArray *stats_ctxt
				 );

			// derive statistics when scalar expression has outer references
			static
			IStatistics *DeriveStatsWithOuterRefs
				(
				 IMemoryPool *mp,
				 CExpressionHandle &exprhdl, // handle attached to the logical expression we want to derive stats for
				 CExpression *expr, // scalar condition used for stats derivation
				 IStatistics *stats, // statistics object of attached expression
				 IStatisticsArray *all_outer_stats, // array of stats objects where outer references are defined
				 IStatistics::EStatsJoinType join_type
				 );
	};
}

#endif // !GPNAUCRATES_CJoinStatsProcessor_H

// EOF

