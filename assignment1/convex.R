#! /usr/bin/env Rscript
png("convex.png", width=700, height=700)
plot(1:10000, 1:10000, type="n")

#points
points(3572,4965)
points(7069,3260)
points(6904,3321)
points(5362,1213)
points(2155,8337)
points(6602,6848)
points(6407,990)
points(6607,5116)
points(2161,440)
points(9027,3631)

#line segments
segments(2155,8337,6602,6848)
segments(2155,8337,2161,440)
segments(6602,6848,9027,3631)
segments(6407,990,2161,440)
segments(6407,990,9027,3631)
dev.off()
