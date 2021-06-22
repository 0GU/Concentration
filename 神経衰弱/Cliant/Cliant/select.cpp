#include "main.h"

IPDATA IP_set() {
	IPDATA ip;

	while (1) {
		ClearDrawScreen();

		DrawString(IP_INPUT_POS_X, IP_INPUT_POS_Y, "IP“ü—Í", GetColor(WHITE));

		ip.d1 = KeyInputNumber(0, 16, 255, 0, FALSE);
		ip.d2 = KeyInputNumber(0, 16, 255, 0, FALSE);
		ip.d3 = KeyInputNumber(0, 16, 255, 0, FALSE);
		ip.d4 = KeyInputNumber(0, 16, 255, 0, FALSE);

		DrawFormatString(IP_POS_X, IP_POS_Y, GetColor(WHITE),
			"IP:%d:%d:%d:%d",
			ip.d1,
			ip.d2,
			ip.d3,
			ip.d4
		);
		DrawString(IP_CONFIRMATION_POS_X, IP_CONFIRMATION_POS_Y, "‚±‚ê‚Å—Ç‚¢‚Å‚·‚©H", GetColor(WHITE));
		DrawString(IP_ANSWER_POS_X, IP_ANSWER_POS_Y, "0...‚¢‚¢‚¦ 1...‚Í‚¢\n>>", GetColor(WHITE));
		if (KeyInputNumber(16, 64, 1, 0, FALSE))break;
	}
	return ip;
}