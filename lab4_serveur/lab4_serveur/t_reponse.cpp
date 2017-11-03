#include "common.h"
#include "t_reponse.h"
#include <thread>


void TReponse::run()
{
	std::thread t(TReponse::threadFctn);
}

void TReponse::threadFctn()
{
	while (true)
	{
		if (!boite_denvoi.empty())
		{
			message_t m = boite_denvoi.front();
			boite_denvoi.pop();
			if (m.dests.empty()) // broadcast message
			{
				for (int i = 0; i < liste_connexions.size(); i++)
				{
					send(liste_connexions[i].sd, m.corps.c_str(), m.corps.size(), 0);
				}
			}
			else // envoyer au destinataire
			{
				while (!m.dests.empty())
				{
					send(m.dests.front(), m.corps.c_str(), m.corps.size(), 0);
					m.dests.pop();
				}
			}
		}
	}
}

