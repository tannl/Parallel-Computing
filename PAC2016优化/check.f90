      subroutine checkRealEqu(v1, v2, res)
         IMPLICIT REAL(8) (V)
         REAL*8, INTENT(IN) :: v1
         REAL*8, INTENT(IN) :: v2
         logical:: res
         parameter(vlimit=1.e-10)

         res = .false.

         if (v1 .eq. 0.0) then
           if (v2 .le. vlimit .and. v2 .ge. -vlimit) then
             res = .true.
           endif
           return
         endif
         
         vratio = (v2 - v1)/v1
         if (vratio .le. vlimit .and. vratio .ge. -vlimit) then
            res = .true.
         endif
      end subroutine checkRealEqu
	  
    LOGICAL FUNCTION IsMissingPheno(Phenotype)
    
        IMPLICIT NONE 
        REAL*8 MissingPheno
        REAL*8, INTENT(IN) :: Phenotype
        PARAMETER(MissingPheno=65.0)
        IsMissingPheno = .FALSE.
        
        IF(ISNAN(Phenotype)) THEN
            IsMissingPheno = .TRUE. 
        ELSE
            IF(ABS(Phenotype-MissingPheno) .LE. 0.0) &
                IsMissingPheno = .TRUE. 
        ENDIF

        RETURN
     
    ENDFUNCTION IsMissingPheno

