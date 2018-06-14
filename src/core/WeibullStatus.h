// $Id: Status.h 522 2005-05-11 18:02:47Z fgroen $

#pragma once

/**
 * Status object.
 * This object is used to store the status of ongoing computations.
 */
class WeibullStatus
{
public:
	WeibullStatus(void);
	~WeibullStatus(void);
	void clear() {status = STATUS_READY; cancel = false;} ///< clear the status
	int getStatus() const {return status;} ///< return the current status
	int getProgress() const {return progress;} ///< return the current progresw
	void setStatus(const int status) {this->status = status;} ///< sets the status
	void setProgress(const int progress) {this->progress = progress;} ///< sets the progress
	int getStatus(char * msg, int len) const;

	void requestCancel() {cancel = true;}
	bool isCancelRequested() const {return cancel;}

	const static int STATUS_BURNIN = 1;
	const static int STATUS_SAMPLE = 2;
	const static int STATUS_READY = 3;
	const static int STATUS_ABORTED = 4;
	const static int STATUS_CONVERGENCE = 5;

private:
	bool cancel;
	int status;
	int progress;
};
