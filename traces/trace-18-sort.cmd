# Test sorting partially random data
option fail 0
option malloc 0
option partial_randomness 1
option enable_timsort 0
new
ih RAND 100000
sort
free
option partial_randomness 0