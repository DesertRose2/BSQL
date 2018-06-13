#include "BSQL.h"

bool MySqlOperation::Poll(MYSQL* mysql, std::chrono::milliseconds timeoutEpoch, int& status) {
	pollfd pfd;
	pfd.fd = mysql_get_socket(mysql);
	pfd.events =
		(status & MYSQL_WAIT_READ ? POLLIN : 0) |
		(status & MYSQL_WAIT_WRITE ? POLLOUT : 0) |
		(status & MYSQL_WAIT_EXCEPT ? POLLPRI : 0);

	const auto res(poll(&pfd, 1, -1));
	const auto waitingForTimeout((status & MYSQL_WAIT_TIMEOUT) != 0);
	const auto oldStatus(status);
	status = 0;
	if (res <= 0) {
		if (waitingForTimeout && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) > timeoutEpoch)
			status |= MYSQL_WAIT_TIMEOUT;
	}
	else {
		if (pfd.revents & POLLIN) status |= MYSQL_WAIT_READ;
		if (pfd.revents & POLLOUT) status |= MYSQL_WAIT_WRITE;
		if (pfd.revents & POLLPRI) status |= MYSQL_WAIT_EXCEPT;
	}
	if (status != 0)
		return true;
	status = oldStatus;
	return false;
}

std::chrono::milliseconds MySqlOperation::GetTimeout(MYSQL* mysql) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + std::chrono::milliseconds(mysql_get_timeout_value_ms(mysql));
}
