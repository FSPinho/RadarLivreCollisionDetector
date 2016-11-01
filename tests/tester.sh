
NOW="$(date)"
DATE_TIME="$(date +'on %d/%m/%Y at %H:%M:%S')"
DATE="$(date +'%d-%m-%Y')"
TIME="$(date +'%H-%M-%S')"
 
BASE_DIR=$(dirname "$0")
TESTS_DIR="statistics/TESTS-ON-$DATE"
TESTS_FILE="$TESTS_DIR/AT-$TIME.txt"
TESTS_PARSER_FILE="$TESTS_DIR/AT-$TIME.csv"
TESTS_ERROR_FILE="$TESTS_DIR/AT-$TIME.error"

EXECUTABLE_FILE="../RadarLivreColisionDetectionCpp"
EXECUTABLE_FILE="../../build-RadarLivreColisionDetectionCpp-Desktop_Qt_5_7_0_GCC_64bit-Debug/RadarLivreColisionDetectionCpp"

PARSER="parser/parser.py"

TEST_VALUES=(100 200 300 400 500 600 700 800 900)
ITERATIONS=5

printf "\nExecuting tests %s\n" "$DATE_TIME"
printf "Creating tests and putting in %s\n\n" "$TESTS_FILE"

# Go to the execution directory
cd "$BASE_DIR"
mkdir -p $TESTS_DIR

# Execution function

function execute {

	local AIRPLANE_COUNT=$1
	local ITERATION_COUNT=$2
	local USE_THREADS=$3
	local USE_AREA_DIVISION=$4

	printf "Executing test (AIRCRAFTS=%d, ITERATIONS=%d, USE_THREADS=%d, USE_AREA_DIVISION=%d)\n" $AIRPLANE_COUNT $ITERATION_COUNT $USE_THREADS $USE_AREA_DIVISION
	
	$EXECUTABLE_FILE $AIRPLANE_COUNT $ITERATION_COUNT $USE_THREADS $USE_AREA_DIVISION >> $TESTS_FILE 2>> $TESTS_ERROR_FILE;

}

function executeSet {

	local ITERATION_COUNT=$1
	local USE_THREADS=$2
	local USE_AREA_DIVISION=$3

	for i in ${TEST_VALUES[@]}
	do
		execute $i $ITERATION_COUNT $USE_THREADS $USE_AREA_DIVISION
	done

}

executeSet 5 0 0
executeSet 5 1 0
executeSet 5 0 1
executeSet 5 1 1

printf "Parsing result...\n"

python $PARSER $TESTS_FILE > $TESTS_PARSER_FILE

printf "Done!\n\n"