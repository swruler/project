/*
 * Custom "many-read one-write" mutex.
 */
#include <iostream>    // Needed for cout
#include <pthread.h>   // Needed for pthread_t
#include <stdio.h>     // Needed for perror
#include <errno.h>     // Needed for errno
#include <stdlib.h>    // Needed for EXIT_FAILURE

using namespace std;

#define handleError(en, msg) errno = en; perror(msg); exit(EXIT_FAILURE);

class ManyReadOneWriteMutex {
private:
	pthread_mutexattr_t mutexAttr;
	pthread_mutex_t mutex;
	int lockCnt;

public:
	ManyReadOneWriteMutex() {
		lockCnt = 0;

		// Initialize mutex attributes.
		int rc;
		if (0 != (rc = pthread_mutexattr_init (&mutexAttr))) {
			cout << "rc " << rc << endl;
			handleError(rc, "Initialize mutex attributes");
		}

		// Initialize the mutex.
		if (0 != (rc = pthread_mutex_init (&mutex, &mutexAttr))) {
			handleError(rc, "Initialize mutex");
		}
		// TODO: Investigate alternative mutex initialization.
		// mutex = PTHREAD_MUTEX_INITIALIZER;
	}

	void writeLock(int tid) {
		while (1) {
			pthread_mutex_lock(&mutex);

			// Remove this debug msg after testing with sample "main" code below.
			cout << "WLK CNT " << lockCnt << " T" << tid << "  ";
			if (0 == lockCnt) {
				break;
			}

			pthread_mutex_unlock(&mutex);
		}
	}

	void writeUnlock(int tid) {
		pthread_mutex_unlock(&mutex);
	}

	void readLock(int tid) {
		pthread_mutex_lock(&mutex);
		lockCnt++;

		// Remove this debug msg after testing with sample "main" code below.
		cout << "RLK CNT " << lockCnt << " T" << tid << "  ";
		pthread_mutex_unlock(&mutex);
	}

	void readUnlock(int tid) {
		pthread_mutex_lock(&mutex);
		lockCnt--;

		// Remove this debug msg after testing with sample "main" code below.
		cout << "RULK CNT " << lockCnt << " T" << tid << endl;
		pthread_mutex_unlock(&mutex);
	}
};

/*
 * Example code to test "many-read one-write" mutex.
 */
#include <unistd.h>    // Needed for usleep

#define PROTECTED_DATA_SIZE 10
#define NUM_THREADS 5

struct tThreadData {
	int* pSharedProtectedData;
	ManyReadOneWriteMutex* pMutex;
	int tid;
};

static void * readThread(void * pParam)
{
	// Wait for all threads to get created.
	usleep(1000000);  // sleep for 1 second

	tThreadData threadData=*(tThreadData*)pParam;
	int i = 0;
	while (i < PROTECTED_DATA_SIZE) {
		threadData.pMutex->readLock(threadData.tid);

		// Validate data.
		if (i != threadData.pSharedProtectedData[i]) {
			cout << "readThread " << i << " CORRUPTED with " << threadData.pSharedProtectedData[i] << " T" << threadData.tid << "  " << endl;
		} else {
			cout << "readThread T" << threadData.tid << " " << threadData.pSharedProtectedData[i] << endl;
		}

		threadData.pMutex->readUnlock(threadData.tid);
		usleep(1000);  // sleep for a tenth of a second to add some non-linear time between reads.
		i++;
	}

	return NULL;
}

static void * writeThread(void * pParam)
{
	// Wait for all threads to get created.
	usleep(1000000);  // sleep for 1 second

	tThreadData threadData=*(tThreadData*)pParam;
	int i = 0;
	while (i < PROTECTED_DATA_SIZE) {
		threadData.pMutex->writeLock(threadData.tid);

		// Validate data.
		if (i != threadData.pSharedProtectedData[i]) {
			cout << "writeThread " << i << " CORRUPTED with " << threadData.pSharedProtectedData[i] << " T" << threadData.tid << "  " << endl;
		} else {
			// Write data.
			threadData.pSharedProtectedData[i] = i;
			cout << "writeThread T" << threadData.tid << " " << i << endl;
		}

		threadData.pMutex->writeUnlock(threadData.tid);
		i++;
	}

	return NULL;
}

int main(int argc, char ** argv) {
	int rc;

	// Protected data and its mutex.
	int sharedProtectedArray[PROTECTED_DATA_SIZE];
	ManyReadOneWriteMutex mutex;

	// Threads.
	tThreadData threadData[NUM_THREADS];
	pthread_attr_t attr[NUM_THREADS];
	pthread_t t[NUM_THREADS];

	int i;

	// Initialize protected data.
	for (i=0; i < PROTECTED_DATA_SIZE; i++) {
		sharedProtectedArray[i] = i;
	}

	// Create threads.
	for (i=0; i < NUM_THREADS; i++) {
		// Initialize threads.
		threadData[i].pSharedProtectedData = &sharedProtectedArray[0];
		threadData[i].pMutex = &mutex;
		threadData[i].tid = i;

		if (0 != (rc=pthread_attr_init(attr+i))) {
			handleError(rc,"Initializing attribute block");
		}

		// Check for first thread.
		if (0 == i) {
			// Create the write thread for the last thread.
			cout << "Create write thread." << endl;
			if (0 != (rc=pthread_create(t+i, attr+i, &writeThread, (void*)(&threadData[i])))) {
				handleError(rc, "Create thread");
			}
		} else {
			// Create a read thread.
			cout << "Create read thread." << endl;
			if (0 != (rc=pthread_create(t+i, attr+i, &readThread, (void*)(&threadData[i])))) {
				handleError(rc, "Create thread");
			}
		}
	}

	// Join all the threads (i.e. wait for all the treads to terminate).
	for (i=0; i<NUM_THREADS; i++) {
		if (0 != (rc=pthread_join(t[i], NULL))) {
			handleError(rc, "Joined thread");
		} else {
			cout << "Joined thread T" << i << endl;
		}

	}

	return 0;
}
