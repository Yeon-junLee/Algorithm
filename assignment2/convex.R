#! /usr/bin/env Rscript
png("convex.png", width=700, height=700)
plot(1:10000, 1:10000, type="n")

#points
points(1671,8587)
points(1906,3957)
points(2614,2725)
points(2951,2414)
points(3007,4978)
points(3337,8367)
points(3464,4509)
points(3769,6124)
points(3807,4699)
points(3868,498)
points(5696,6757)
points(6026,5490)
points(6350,4908)
points(6962,1492)
points(7099,726)
points(7947,9489)
points(8813,4839)
points(9492,5127)
points(9801,6620)
points(9882,4280)

#line segments
segments(1671,8587,7947,9489)
segments(7947,9489,9801,6620)
segments(9801,6620,9882,4280)
segments(9882,4280,7099,726)
segments(7099,726,3868,498)
segments(3868,498,1906,3957)
segments(1906,3957,1671,8587)
dev.off()
