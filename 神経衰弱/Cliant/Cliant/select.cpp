#include "main.h"

IPDATA IP_set() {
	IPDATA ip;

	while (1) {
		ClearDrawScreen();

		DrawString(0, 0, "IP“ü—Í", GetColor(255, 255, 255));

		ip.d1 = KeyInputNumber(0, 16, 255, 0, FALSE);
		ip.d2 = KeyInputNumber(0, 16, 255, 0, FALSE);
		ip.d3 = KeyInputNumber(0, 16, 255, 0, FALSE);
		ip.d4 = KeyInputNumber(0, 16, 255, 0, FALSE);

		DrawFormatString(0, 16, GetColor(255, 255, 255),
			"IP:%d:%d:%d:%d",
			ip.d1,
			ip.d2,
			ip.d3,
			ip.d4
		);
		DrawString(0, 32, "‚±‚ê‚Å—Ç‚¢‚Å‚·‚©H", GetColor(255, 255, 255));
		DrawString(0, 48, "0...‚¢‚¢‚¦ 1...‚Í‚¢\n>>", GetColor(255, 255, 255));
		if (KeyInputNumber(16, 64, 1, 0, FALSE))break;
	}
	return ip;
}