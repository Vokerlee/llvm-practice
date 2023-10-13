; ModuleID = 'gameoflife.c'
source_filename = "gameoflife.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @init_func(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store ptr %1, ptr %6, align 8
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  store i32 -21731286, ptr %9, align 4
  %13 = load i32, ptr %9, align 4
  call void @sgl_srand(i32 noundef %13)
  store i32 0, ptr %10, align 4
  br label %14

14:                                               ; preds = %57, %4
  %15 = load i32, ptr %10, align 4
  %16 = load i32, ptr %8, align 4
  %17 = icmp slt i32 %15, %16
  br i1 %17, label %18, label %60

18:                                               ; preds = %14
  store i32 0, ptr %11, align 4
  br label %19

19:                                               ; preds = %53, %18
  %20 = load i32, ptr %11, align 4
  %21 = load i32, ptr %7, align 4
  %22 = icmp slt i32 %20, %21
  br i1 %22, label %23, label %56

23:                                               ; preds = %19
  %24 = load i32, ptr %10, align 4
  %25 = load i32, ptr %7, align 4
  %26 = mul nsw i32 %24, %25
  %27 = load i32, ptr %11, align 4
  %28 = add nsw i32 %26, %27
  %29 = mul nsw i32 %28, 4
  store i32 %29, ptr %12, align 4
  %30 = load ptr, ptr %6, align 8
  %31 = load i32, ptr %12, align 4
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds i8, ptr %30, i64 %32
  store i8 -128, ptr %33, align 1
  %34 = load ptr, ptr %6, align 8
  %35 = load i32, ptr %12, align 4
  %36 = add nsw i32 %35, 1
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds i8, ptr %34, i64 %37
  store i8 0, ptr %38, align 1
  %39 = load ptr, ptr %6, align 8
  %40 = load i32, ptr %12, align 4
  %41 = add nsw i32 %40, 2
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds i8, ptr %39, i64 %42
  store i8 -128, ptr %43, align 1
  %44 = call i32 (...) @sgl_rand()
  %45 = srem i32 %44, 2
  %46 = mul nsw i32 255, %45
  %47 = trunc i32 %46 to i8
  %48 = load ptr, ptr %6, align 8
  %49 = load i32, ptr %12, align 4
  %50 = add nsw i32 %49, 3
  %51 = sext i32 %50 to i64
  %52 = getelementptr inbounds i8, ptr %48, i64 %51
  store i8 %47, ptr %52, align 1
  br label %53

53:                                               ; preds = %23
  %54 = load i32, ptr %11, align 4
  %55 = add nsw i32 %54, 1
  store i32 %55, ptr %11, align 4
  br label %19, !llvm.loop !6

56:                                               ; preds = %19
  br label %57

57:                                               ; preds = %56
  %58 = load i32, ptr %10, align 4
  %59 = add nsw i32 %58, 1
  store i32 %59, ptr %10, align 4
  br label %14, !llvm.loop !8

60:                                               ; preds = %14
  ret void
}

declare void @sgl_srand(i32 noundef) #1

declare i32 @sgl_rand(...) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @draw_func(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store ptr %1, ptr %6, align 8
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  store i32 0, ptr %9, align 4
  br label %16

16:                                               ; preds = %153, %4
  %17 = load i32, ptr %9, align 4
  %18 = load i32, ptr %8, align 4
  %19 = icmp slt i32 %17, %18
  br i1 %19, label %20, label %156

20:                                               ; preds = %16
  store i32 0, ptr %10, align 4
  br label %21

21:                                               ; preds = %149, %20
  %22 = load i32, ptr %10, align 4
  %23 = load i32, ptr %7, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %152

25:                                               ; preds = %21
  %26 = load i32, ptr %9, align 4
  %27 = load i32, ptr %7, align 4
  %28 = mul nsw i32 %26, %27
  %29 = load i32, ptr %10, align 4
  %30 = add nsw i32 %28, %29
  %31 = mul nsw i32 %30, 4
  store i32 %31, ptr %11, align 4
  store i32 0, ptr %12, align 4
  store i32 -1, ptr %13, align 4
  br label %32

32:                                               ; preds = %93, %25
  %33 = load i32, ptr %13, align 4
  %34 = icmp sle i32 %33, 1
  br i1 %34, label %35, label %96

35:                                               ; preds = %32
  store i32 -1, ptr %14, align 4
  br label %36

36:                                               ; preds = %89, %35
  %37 = load i32, ptr %14, align 4
  %38 = icmp sle i32 %37, 1
  br i1 %38, label %39, label %92

39:                                               ; preds = %36
  %40 = load i32, ptr %14, align 4
  %41 = icmp eq i32 %40, 0
  br i1 %41, label %42, label %46

42:                                               ; preds = %39
  %43 = load i32, ptr %13, align 4
  %44 = icmp eq i32 %43, 0
  br i1 %44, label %45, label %46

45:                                               ; preds = %42
  br label %89

46:                                               ; preds = %42, %39
  %47 = load i32, ptr %9, align 4
  %48 = load i32, ptr %13, align 4
  %49 = add nsw i32 %47, %48
  %50 = load i32, ptr %8, align 4
  %51 = add nsw i32 %49, %50
  %52 = load i32, ptr %8, align 4
  %53 = srem i32 %51, %52
  %54 = load i32, ptr %7, align 4
  %55 = mul nsw i32 %53, %54
  %56 = load i32, ptr %10, align 4
  %57 = load i32, ptr %14, align 4
  %58 = add nsw i32 %56, %57
  %59 = load i32, ptr %7, align 4
  %60 = add nsw i32 %58, %59
  %61 = load i32, ptr %7, align 4
  %62 = srem i32 %60, %61
  %63 = add nsw i32 %55, %62
  %64 = mul nsw i32 %63, 4
  store i32 %64, ptr %15, align 4
  %65 = load i32, ptr %15, align 4
  %66 = icmp sge i32 %65, 0
  br i1 %66, label %67, label %88

67:                                               ; preds = %46
  %68 = load i32, ptr %15, align 4
  %69 = load i32, ptr %7, align 4
  %70 = load i32, ptr %8, align 4
  %71 = mul nsw i32 %69, %70
  %72 = mul nsw i32 %71, 4
  %73 = sub nsw i32 %72, 1
  %74 = icmp slt i32 %68, %73
  br i1 %74, label %75, label %88

75:                                               ; preds = %67
  %76 = load ptr, ptr %5, align 8
  %77 = load i32, ptr %15, align 4
  %78 = add nsw i32 %77, 3
  %79 = sext i32 %78 to i64
  %80 = getelementptr inbounds i8, ptr %76, i64 %79
  %81 = load i8, ptr %80, align 1
  %82 = zext i8 %81 to i32
  %83 = icmp ne i32 %82, 0
  br i1 %83, label %84, label %87

84:                                               ; preds = %75
  %85 = load i32, ptr %12, align 4
  %86 = add nsw i32 %85, 1
  store i32 %86, ptr %12, align 4
  br label %87

87:                                               ; preds = %84, %75
  br label %88

88:                                               ; preds = %87, %67, %46
  br label %89

89:                                               ; preds = %88, %45
  %90 = load i32, ptr %14, align 4
  %91 = add nsw i32 %90, 1
  store i32 %91, ptr %14, align 4
  br label %36, !llvm.loop !9

92:                                               ; preds = %36
  br label %93

93:                                               ; preds = %92
  %94 = load i32, ptr %13, align 4
  %95 = add nsw i32 %94, 1
  store i32 %95, ptr %13, align 4
  br label %32, !llvm.loop !10

96:                                               ; preds = %32
  %97 = load ptr, ptr %5, align 8
  %98 = load i32, ptr %11, align 4
  %99 = add nsw i32 %98, 3
  %100 = sext i32 %99 to i64
  %101 = getelementptr inbounds i8, ptr %97, i64 %100
  %102 = load i8, ptr %101, align 1
  %103 = zext i8 %102 to i32
  %104 = icmp eq i32 %103, 255
  br i1 %104, label %105, label %117

105:                                              ; preds = %96
  %106 = load i32, ptr %12, align 4
  %107 = icmp slt i32 %106, 2
  br i1 %107, label %111, label %108

108:                                              ; preds = %105
  %109 = load i32, ptr %12, align 4
  %110 = icmp sgt i32 %109, 3
  br i1 %110, label %111, label %117

111:                                              ; preds = %108, %105
  %112 = load ptr, ptr %6, align 8
  %113 = load i32, ptr %11, align 4
  %114 = add nsw i32 %113, 3
  %115 = sext i32 %114 to i64
  %116 = getelementptr inbounds i8, ptr %112, i64 %115
  store i8 0, ptr %116, align 1
  br label %148

117:                                              ; preds = %108, %96
  %118 = load ptr, ptr %5, align 8
  %119 = load i32, ptr %11, align 4
  %120 = add nsw i32 %119, 3
  %121 = sext i32 %120 to i64
  %122 = getelementptr inbounds i8, ptr %118, i64 %121
  %123 = load i8, ptr %122, align 1
  %124 = zext i8 %123 to i32
  %125 = icmp eq i32 %124, 0
  br i1 %125, label %126, label %135

126:                                              ; preds = %117
  %127 = load i32, ptr %12, align 4
  %128 = icmp eq i32 %127, 3
  br i1 %128, label %129, label %135

129:                                              ; preds = %126
  %130 = load ptr, ptr %6, align 8
  %131 = load i32, ptr %11, align 4
  %132 = add nsw i32 %131, 3
  %133 = sext i32 %132 to i64
  %134 = getelementptr inbounds i8, ptr %130, i64 %133
  store i8 -1, ptr %134, align 1
  br label %147

135:                                              ; preds = %126, %117
  %136 = load ptr, ptr %5, align 8
  %137 = load i32, ptr %11, align 4
  %138 = add nsw i32 %137, 3
  %139 = sext i32 %138 to i64
  %140 = getelementptr inbounds i8, ptr %136, i64 %139
  %141 = load i8, ptr %140, align 1
  %142 = load ptr, ptr %6, align 8
  %143 = load i32, ptr %11, align 4
  %144 = add nsw i32 %143, 3
  %145 = sext i32 %144 to i64
  %146 = getelementptr inbounds i8, ptr %142, i64 %145
  store i8 %141, ptr %146, align 1
  br label %147

147:                                              ; preds = %135, %129
  br label %148

148:                                              ; preds = %147, %111
  br label %149

149:                                              ; preds = %148
  %150 = load i32, ptr %10, align 4
  %151 = add nsw i32 %150, 1
  store i32 %151, ptr %10, align 4
  br label %21, !llvm.loop !11

152:                                              ; preds = %21
  br label %153

153:                                              ; preds = %152
  %154 = load i32, ptr %9, align 4
  %155 = add nsw i32 %154, 1
  store i32 %155, ptr %9, align 4
  br label %16, !llvm.loop !12

156:                                              ; preds = %16
  ret void
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
