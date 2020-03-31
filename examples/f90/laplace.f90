MODULE laplace
  
CONTAINS
  SUBROUTINE redblack(a,residue)
    INTEGER, PARAMETER :: N = 20
    ! Due to limitations within the NAG F90 compiler,
    ! N must be declared constant, rather than passed
    ! into the subroutine
    REAL,    INTENT(out) :: residue
    REAL,    DIMENSION (N,N), INTENT(inout) :: a

    REAL,    DIMENSION (N,N) :: old_a
    LOGICAL, DIMENSION (N,N) :: even

    DO j = 1,N
      DO i = 1,N
        EVEN (i,j) = (MOD(i+j,2) == 0)
      ENDDO
    ENDDO

!---- Red-black relaxation:
    old_a = a
    WHERE (even(2:N-1,2:N-1))
      a (2:N-1,2:N-1) = 0.4 * a (2:N-1,2:N-1)                             &  
                      - 0.6 * 0.25 * ( a(1:N-2,2:N-1) + a(3:N,2:N-1)      &
                                     + a(2:N-1,1:N-2) + a(2:N-1,3:N))
    ELSEWHERE
      a (2:N-1,2:N-1) = 0.4 * a (2:N-1,2:N-1)                             & 
                      - 0.6 * 0.25 * ( a(1:N-2,2:N-1) + a(3:N,2:N-1)      &
                                     + a(2:N-1,1:N-2) + a(2:N-1,3:N))
    ENDWHERE

    residue = 0
    DO j = 1,N
      DO i = 1,N
        IF (ABS(a(i,j)-old_a(i,j)) > residue) THEN
          residue = ABS(a(i,j)-old_a(i,j))
        ENDIF
      ENDDO
    ENDDO
  END SUBROUTINE

END MODULE laplace
