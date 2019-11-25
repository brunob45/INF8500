#ifndef THROUGHPUT_IF_H
#define THROUGHPUT_IF_H

typedef struct {
	double throughput;
	double delta_time;
} throughput_stat;

class throughput_if {
	public:

		virtual ~throughput_if() {}
		virtual throughput_stat get_throughput() = 0;
		virtual void reset() = 0;
};

#endif
