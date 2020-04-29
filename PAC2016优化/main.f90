      PROGRAM MAIN
      INTEGER, PARAMETER:: NP=2147483647
!      INTEGER, PARAMETER:: NObs=2147483647
!      INTEGER, PARAMETER:: NObs=1073741824
      INTEGER, PARAMETER:: NObs=4096
      REAL*8, PARAMETER:: minx=65.0
      REAL*8, PARAMETER:: miny=66.0
      REAL*8:: x, y, z, rcsum, rcsum1
      REAL*8, DIMENSION(NP):: X1,X2
      INTEGER:: i,N,nn
      LOGICAL:: lreal
      REAL*8,EXTERNAL :: Covariance
      REAL*8,EXTERNAL :: Covariance_check
     
      INTEGER :: count1, count_rate1, count_max1
      INTEGER :: count2, count_rate2, count_max2
      call random_number(z)
      N=floor(5*z)+6
!     data initialization
      do i = 1, NObs
         if(mod(i,N)==0)then
              X1(i)=minx
          else
              call random_number(x)
              X1(i) = minx+x 
          endif
      end do
      do i = 1, NObs
        if(mod(i,N)==0)then
          X2(i)=minx
        else
              call random_number(y)
              X2(i) = minx+y
        endif
      end do
      if (NObs==4096)then
         nn=524288
      else 
         nn=2
      endif
!     data process 
      CALL SYSTEM_CLOCK(count1, count_rate1, count_max1)
      do i = 1, nn
      rcsum=Covariance(NObs, X1, X2)
      end do
      CALL SYSTEM_CLOCK(count2, count_rate2, count_max2)
      print *, "total time: ", (count2-count1)*1.0/count_rate2

!     calculate the result of original version      
      
      rcsum1=Covariance_check(NObs, X1, X2)
!     correctness verification

      call checkRealEqu(rcsum, rcsum1, lreal)

      IF ( lreal ) then        
          print *, "verification: correct"
      ELSE
          print *, "verification: incorrect, cal=", rcsum, " orginal=", rcsum1
      ENDIF

      END

