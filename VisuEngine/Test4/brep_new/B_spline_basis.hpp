namespace B_spline_basis
{

template <typename STORETYPE, typename COMPUTETYPE>
COMPUTETYPE GetBasis(unsigned int i, unsigned int p, COMPUTETYPE u, STORETYPE* p_Knots)
{
	static int depth = 0;
	depth++;

	COMPUTETYPE retVal;

	/*fprintf(stderr, "u=%f\n", u);
	fprintf(stderr, "p_Knots[i]=%f\n", p_Knots[i]);
	fprintf(stderr, "p_Knots[i+1]=%f\n", p_Knots[i+1]);
	if (u >= static_cast<COMPUTETYPE>(p_Knots[i])) {
		fprintf(stderr, "u >= p_Knots[i] ? : true\n");
	} else {
		fprintf(stderr, "u >= p_Knots[i] ? : false\n");
	}*/

	if (p == 0) {

		/*
		COMPUTETYPE curKnot = static_cast<COMPUTETYPE>(p_Knots[i]);
		COMPUTETYPE nxtKnot = static_cast<COMPUTETYPE>(p_Knots[i+1]);
		LOG(DEBUG4) << "u   : " << GetBinaryRepresentation(sizeof(u), (void*)&u) << "\t" << u << cls_logger::endl;
		LOG(DEBUG4) << "t[" << i << "]: " << GetBinaryRepresentation(sizeof(curKnot), (void*)&curKnot) << "\t" << curKnot << cls_logger::endl;
		LOG(DEBUG4) << "t[" << i+1 << "]: " << GetBinaryRepresentation(sizeof(nxtKnot), (void*)&nxtKnot) << "\t" << nxtKnot << cls_logger::endl;
		*/
		
		if (u >= static_cast<COMPUTETYPE>(p_Knots[i]) && u < static_cast<COMPUTETYPE>(p_Knots[i+1])) { //TODO <= ?
			depth--;
			retVal = static_cast<COMPUTETYPE>(1.);

			LOG(DEBUG4) << depth << "| 1 | GetBasis(" << i << "," << p << "," << u << ") = "
			            << retVal << cls_logger::endl;

			return retVal;
		} else {
			depth--;
			retVal = static_cast<COMPUTETYPE>(0.);

			LOG(DEBUG4) << depth << "| 2 | GetBasis(" << i << "," << p << "," << u << ") = "
			            << retVal << cls_logger::endl;

			return retVal;
		}
	} else {
		COMPUTETYPE Bi = GetBasis(i, p-1, u, p_Knots);
		COMPUTETYPE Biplus1 = GetBasis(i+1, p-1, u, p_Knots);

		depth--;

		COMPUTETYPE ti = static_cast<COMPUTETYPE>(p_Knots[i]);
		COMPUTETYPE tipluspplus1 = static_cast<COMPUTETYPE>(p_Knots[i+p+1]);

		COMPUTETYPE denom1 = (static_cast<COMPUTETYPE>(p_Knots[i+p]) - ti);
		COMPUTETYPE denom2 = (tipluspplus1 - static_cast<COMPUTETYPE>(p_Knots[i+1]));

		COMPUTETYPE term1 = static_cast<COMPUTETYPE>(0.);
		if (denom1 != static_cast<COMPUTETYPE>(0.)) {
			term1 = (u - ti) * Bi / denom1;
		} else {
			//LOG(WARNING) << "Denominator = 0" << cls_logger::endl;
		}

		COMPUTETYPE term2 = static_cast<COMPUTETYPE>(0.);
		if (denom2 != static_cast<COMPUTETYPE>(0.)) {
			term2 = (tipluspplus1 - u) * Biplus1 / denom2;
		} else {
			//LOG(WARNING) << "Denominator = 0" << cls_logger::endl;
		}

		retVal = term1 + term2;

		LOG(DEBUG4) << depth << "| 3 | GetBasis(" << i << "," << p << "," << u << ") = "
		            << retVal << cls_logger::endl;

		return retVal;
	}
}

}
