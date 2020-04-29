    REAL*8 FUNCTION Covariance_check(NObs, X1, X2)

        IMPLICIT NONE

        INTEGER, INTENT(IN) :: NObs
        REAL*8,    INTENT(IN) :: X1(NObs)
        REAL*8,    INTENT(IN) :: X2(NObs)


        INTEGER :: IObs, NumMissing
        REAL*8 :: MeanX1, MeanX2
        LOGICAL,EXTERNAL :: IsMissingPheno
        NumMissing=0
        MeanX1=0.0
        MeanX2=0.0
        Covariance_check=0.0 

        DO IObs=1, NObs
            IF(IsMissingPheno(X1(IObs)) .OR. &
                IsMissingPheno(X2(IObs))) THEN
                NumMissing=NumMissing+1
       
            ELSE
                MeanX1=MeanX1+X1(IObs)
                MeanX2=MeanX2+X2(IObs)

                Covariance_check=Covariance_check+X1(IObs)*X2(IObs)
     
            ENDIF
            
        ENDDO

        MeanX1=MeanX1/(NObs-NumMissing)
        MeanX2=MeanX2/(NObs-NumMissing)

        IF(NObs-NumMissing .LE. 0) THEN
            Covariance_check=0.0
        ELSE
            Covariance_check=Covariance_check/(NObs-NumMissing)-MeanX1*MeanX2
   
        ENDIF


        RETURN

    END FUNCTION Covariance_check
